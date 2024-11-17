#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>
#include <functional>
#include "Database/Serializable/Task.hpp"
#include "Database/Serializable/ISerializable.hpp"

// Base class for a threaded file queue (for serialization and deserialization).
class ThreadedFileQueue {
public:
  using TaskCallback = std::function<void(std::shared_ptr<ISerializable>)>;

  ThreadedFileQueue();
  virtual ~ThreadedFileQueue();

  // Set the base path for file operations
  void setBasepath(const std::string_view basePath);

  // Get the current base path
  std::string getBasepath() const;

  // Enqueue a task to the queue
  bool enqueue(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback);

  // Get the current progress (0 to 100)
  float getProgress() const;

  // Get the current file being processed
  std::string_view getCurrentFile() const;

  // Abort current processing
  void abort();

  void terminate();

  // Reset and clear the queue, also aborts current tasks
  void reset();

  bool hasTasks() const { return m_taskQueue.size() > 0; }
protected:
  // The worker loop, to be implemented in subclasses (serialization/deserialization)
  virtual void processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) = 0;

  // The worker thread method
  void workerLoop();

  // Current base path for file operations
  std::string m_basePath;

  // Progress tracking
  int m_totalTasks = 0;
  int m_completedTasks = 0;

  // Task queue
  std::queue<Task> m_taskQueue;

  // Worker thread
  std::thread m_workerThread;

  // Mutex and condition variable for thread synchronization
  mutable std::mutex m_mutex;
  std::condition_variable m_cv;

  // Current file being processed
  std::string m_currentFile;

  // Abort flag
  bool m_abortRequested = false;
};
