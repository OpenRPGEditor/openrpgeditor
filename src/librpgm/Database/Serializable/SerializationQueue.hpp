#pragma once
#include "Database/Serializable/ThreadedFileQueue.hpp"

class SerializationQueue : public ThreadedFileQueue {
public:
  SerializationQueue() = default;
  ~SerializationQueue() override;

protected:
  // Implement the processTask method for serialization
  void processTask(const std::shared_ptr<ISerializable>& fileData, const TaskCallback& callback) override;
};
