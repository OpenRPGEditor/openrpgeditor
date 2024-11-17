#pragma once
#include "Database/Serializable/ThreadedFileQueue.hpp"

class SerializationQueue : public ThreadedFileQueue {
  SerializationQueue() = default;
public:
  SerializationQueue(const SerializationQueue&) = delete;
  SerializationQueue(SerializationQueue&&) = delete;
  SerializationQueue& operator=(const SerializationQueue&) = delete;
  SerializationQueue& operator=(SerializationQueue&&) = delete;

  static SerializationQueue& instance();

protected:
  // Implement the processTask method for serialization
  void processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) override;
};
