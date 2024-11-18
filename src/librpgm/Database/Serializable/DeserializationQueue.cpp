#include "Database/Serializable/DeserializationQueue.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

void DeserializationQueue::processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) {
  m_currentFile = fileData->filepath();

  fs::path filePath = fs::path(m_basePath) / fs::path(m_currentFile);
  if (std::ifstream inFile(filePath, std::ios::binary); inFile) {
    {
      std::lock_guard lock(m_mutex);
      fileData->deserialize(inFile);
      m_completedTasks++;
    }
    callback(fileData);
  } else {
    std::cerr << "Error: Failed to open file for deserialization: " << filePath << std::endl;
  }
}

DeserializationQueue& DeserializationQueue::instance() {
  static DeserializationQueue instance;
  return instance;
}