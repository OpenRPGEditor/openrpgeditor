#include "Database/Serializable/DeserializationQueue.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>

namespace fs = std::filesystem;

DeserializationQueue::~DeserializationQueue() {
  abort();
  if (m_workerThread.joinable()) {
    m_workerThread.join();
  }
}

void DeserializationQueue::processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) {
  m_currentFile = fileData->getFilePath();

  fs::path filePath = fs::path(m_basePath) / fs::path(m_currentFile);
  if (std::ifstream inFile(filePath, std::ios::binary); inFile) {
    fileData->deserialize(inFile);
    {
      std::lock_guard<std::mutex> lock(m_mutex);
      m_completedTasks++;
    }
    callback(fileData);
  } else {
    std::cerr << "Error: Failed to open file for deserialization: " << filePath << std::endl;
  }
}
