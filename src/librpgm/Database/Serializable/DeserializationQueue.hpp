#pragma once
#include "Database/Serializable/ThreadedFileQueue.hpp"

class DeserializationQueue : public ThreadedFileQueue {
  DeserializationQueue() = default;

public:
  DeserializationQueue(const DeserializationQueue&) = delete;
  DeserializationQueue(DeserializationQueue&&) = delete;
  DeserializationQueue& operator=(const DeserializationQueue&) = delete;
  DeserializationQueue& operator=(DeserializationQueue&&) = delete;

  static DeserializationQueue& instance();

protected:
  // Implement the processTask method for deserialization
  void processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) override;
};
