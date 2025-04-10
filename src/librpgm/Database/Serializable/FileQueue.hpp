#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/Serializable/Task.hpp"
#include <condition_variable>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

// Base class for a threaded file queue (for serialization and deserialization).
class FileQueue {
public:
  using TaskCallback = std::function<void(std::shared_ptr<ISerializable>)>;

  FileQueue();
  virtual ~FileQueue();

  // Set the base path for file operations
  void setBasepath(const std::string_view basePath);

  // Get the current base path
  std::string getBasepath() const;

  // Enqueue a task to the queue
  bool enqueue(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback, bool sync = false);

  // Reset and clear the queue, also aborts current tasks
  void reset();

  bool hasTasks() const { return !m_taskQueue.empty(); }

  // Get the current progress (0 to 100)
  float progress() const;

  int currentTaskNumber() const { return m_completedTasks + 1; }
  int totalTasks() const { return m_totalTasks; }
  std::string_view currentTaskName() const { return m_currentFilePath; }

  bool hasTask(const std::string_view path) const {
    return std::ranges::find_if(m_taskQueue, [&path](const Task& task) { return task.filepath() == path; }) != m_taskQueue.end();
  }

  // The worker thread method
  void proc();

  static FileQueue& instance() {
    static FileQueue instance;
    return instance;
  }

  ISerializable::Operation operationType() const { return m_currentOperation; }

protected:
  // The worker loop, to be implemented in subclasses (serialization/deserialization)
  void processReadTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) const;
  void processWriteTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) const;
  static bool pushTask(const std::shared_ptr<ISerializable>& fileData, const std::deque<Task>& queue);

  // Current base path for file operations
  std::string m_basePath;

  // Progress tracking
  int m_totalTasks = 0;
  int m_completedTasks = 0;

  // Task queue
  std::deque<Task> m_taskQueue;
  std::deque<Task> m_pendingQueue;

  ISerializable::Operation m_currentOperation = ISerializable::Operation::Read;
  ISerializable::Operation m_pendingOperation = ISerializable::Operation::Write;
  std::string m_currentFilePath;
};
