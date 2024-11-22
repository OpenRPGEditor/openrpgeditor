#include "Database/Serializable/SerializationQueue.hpp"
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <random>

namespace fs = std::filesystem;

std::string generateRandomFilename(const std::string& prefix, int length = 10) {
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

void SerializationQueue::processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) {
  m_currentFile = fileData->filepath();

  if (m_basePath.empty()) {
    return;
  }

  // Generate a temporary file path in the base directory
  fs::path tempFilePath = fs::temp_directory_path() / fs::path(generateRandomFilename("ore"));
  fs::path finalFilePath = fs::path(m_basePath) / fs::path(fileData->filepath());

  std::ofstream outFile(tempFilePath, std::ios::binary);
  if (outFile) {
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
      {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_completedTasks++;
      }
      callback(fileData); // Callback with the successfully serialized data
    } catch (const std::exception& e) {
      try {
        if (fs::exists(finalFilePath)) {
          fs::remove(finalFilePath); // Remove the existing file if it exists
        }
        fs::copy(tempFilePath, finalFilePath); // Rename temp file to the target file
        fs::remove(tempFilePath);
        {
          std::lock_guard<std::mutex> lock(m_mutex);
          m_completedTasks++;
        }
        callback(fileData); // Callback with the successfully serialized data
      } catch (const std::exception& e) {}
    }
  }
}

SerializationQueue& SerializationQueue::instance() {
  static SerializationQueue instance;
  return instance;
}