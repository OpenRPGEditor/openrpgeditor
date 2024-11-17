#pragma once
#include "Database/Serializable/ThreadedFileQueue.hpp"

class DeserializationQueue : public ThreadedFileQueue {
public:
  DeserializationQueue() = default;
  ~DeserializationQueue() override;

protected:
  // Implement the processTask method for deserialization
  void processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) override;
};
