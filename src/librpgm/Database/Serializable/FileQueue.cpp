#include "Database/Serializable/FileQueue.hpp"

#include "Database/Log.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>

#include <filesystem>
#include <iostream>
#include <magic_enum/magic_enum.hpp>
#include <ranges>

FileQueue::FileQueue() {}
FileQueue::~FileQueue() {}

void FileQueue::setBasepath(const std::string_view basePath) { m_basePath = basePath; }

std::string FileQueue::getBasepath() const { return m_basePath; }

bool FileQueue::enqueue(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback, const bool sync) {
  if (m_basePath.empty()) {
    return false;
  }

  if (sync) {
    if (fileData->operation() == ISerializable::Operation::Read) {
      processReadTask(fileData, callback);
    } else if (fileData->operation() == ISerializable::Operation::Write) {
      processWriteTask(fileData, callback);
    }
    return true;
  }

  if (!m_taskQueue.empty() && m_currentOperation != fileData->operation()) {
    RPGM_WARN("Cannot add {} tasks while {} tasks are pending!", magic_enum::enum_name(fileData->operation()), magic_enum::enum_name(m_currentOperation));
    return false;
  }

  if (const auto& it = std::ranges::find_if(m_taskQueue, [&fileData](const Task& task) { return task.filepath() == fileData->filepath(); }); it != m_taskQueue.end()) {
    RPGM_WARN("Adding a new {} task for {} which is already in the queue for {}!", magic_enum::enum_name(fileData->operation()), fileData->filepath(), magic_enum::enum_name(it->operation()));
    return false;
  }

  m_taskQueue.emplace_back(fileData, callback);
  m_totalTasks++;
  m_currentOperation = fileData->operation();

  return true;
}

float FileQueue::progress() const { return m_totalTasks == 0 ? 0.f : (static_cast<float>(m_completedTasks) / static_cast<float>(m_totalTasks) * 100.f); }

void FileQueue::reset() {
  m_totalTasks = 0;
  m_completedTasks = 0;
  m_taskQueue = {};
}

void FileQueue::proc() {
  if (m_taskQueue.empty()) {
    return;
  }

  const Task task = std::move(m_taskQueue.front());
  m_taskQueue.pop_front();
  m_currentFilePath = task.filepath();
  // Process the task based on the specific queue type
  if (task.operation() == ISerializable::Operation::Read) {
    processReadTask(task.fileData(), task.callback());
  } else if (task.operation() == ISerializable::Operation::Write) {
    processWriteTask(task.fileData(), task.callback());
  }
  m_completedTasks++;
}

namespace fs = std::filesystem;

std::string generateRandomFilename(const std::string& prefix, const int length = 10) {
  static const std::string allowedChars =
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "0123456789";

  std::random_device rd;
  std::mt19937 generator(rd());
  std::uniform_int_distribution<int> distribution(0, static_cast<int>(allowedChars.size()) - 1);

  std::string randomString;
  for (int i = 0; i < length; ++i) {
    randomString += allowedChars[distribution(generator)];
  }

  return prefix + randomString;
}

void FileQueue::processReadTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) const {

  const fs::path filePath = fs::path(m_basePath) / fs::path(fileData->filepath());
  if (std::ifstream inFile(filePath, std::ios::binary); inFile) {
    fileData->deserialize(inFile);
    callback(fileData);
  } else {
    RPGM_WARN("Error: Failed to open file for deserialization: {}", fileData->filepath());
  }
}

void FileQueue::processWriteTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) const {
  if (m_basePath.empty()) {
    return;
  }

  // Generate a temporary file path in the base directory
  const fs::path tempFilePath = fs::temp_directory_path() / fs::path(generateRandomFilename("ore"));
  const fs::path finalFilePath = fs::path(m_basePath) / fs::path(fileData->filepath());

  if (std::ofstream outFile(tempFilePath, std::ios::binary); outFile) {
    // Serialize data to the temporary file
    fileData->serialize(outFile);

    // Now that the data is serialized, attempt to move the temporary file to the final destination
    outFile.close();

    // Rename the temporary file to the target destination if the write succeeded
    try {
      if (fs::exists(finalFilePath)) {
        fs::remove(finalFilePath); // Remove the existing file if it exists
      }

      fs::rename(tempFilePath, finalFilePath);
      callback(fileData); // Callback with the successfully serialized data
    } catch (const std::exception& e) {
      try {
        if (fs::exists(finalFilePath)) {
          fs::remove(finalFilePath); // Remove the existing file if it exists
        }
        fs::copy(tempFilePath, finalFilePath); // Rename temp file to the target file
        fs::remove(tempFilePath);
        callback(fileData); // Callback with the successfully serialized data
      } catch (const std::exception& e) {}
    }
  }
}
