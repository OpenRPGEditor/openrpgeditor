#include "Database/Serializable/ThreadedFileQueue.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

ThreadedFileQueue::ThreadedFileQueue() { m_workerThread = std::thread(&ThreadedFileQueue::workerLoop, this); }

ThreadedFileQueue::~ThreadedFileQueue() {
  abort();
  if (m_workerThread.joinable()) {
    m_workerThread.join();
  }
}

void ThreadedFileQueue::setBasePath(const std::string& basePath) {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_basePath = basePath;
}

std::string ThreadedFileQueue::getBasePath() const { return m_basePath; }

bool ThreadedFileQueue::enqueue(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) {
  {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_basePath.empty()) {
      return false;
    }

    m_taskQueue.emplace(fileData, callback);
    m_totalTasks++;
  }
  m_cv.notify_all();
  return true;
}

float ThreadedFileQueue::getProgress() const {
  return m_totalTasks == 0 ? 0.f : (static_cast<double>(m_completedTasks) / m_totalTasks) * 100.f;
}

std::string_view ThreadedFileQueue::getCurrentFile() const { return m_currentFile; }

void ThreadedFileQueue::abort() {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_abortRequested = true;
  m_cv.notify_all();
}

void ThreadedFileQueue::reset() {
  std::lock_guard<std::mutex> lock(m_mutex);
  m_totalTasks = 0;
  m_completedTasks = 0;
  m_taskQueue = {};
  m_currentFile.clear();
  m_basePath.clear();
}

void ThreadedFileQueue::workerLoop() {
  while (!m_abortRequested) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this]() { return !m_taskQueue.empty() || m_abortRequested; });

    if (m_abortRequested && m_taskQueue.empty())
      break;

    Task task = std::move(m_taskQueue.front());
    m_taskQueue.pop();
    lock.unlock();

    // Process the task based on the specific queue type
    processTask(task.fileData, task.callback);
  }
}
