#pragma once

#include <memory>
#include <functional>

// Forward declaration of ISerializable, which represents a file data object.
class ISerializable;

// Task metadata and associated callback
struct Task {
  std::shared_ptr<ISerializable> fileData;                      // The file data (serializable object)
  std::function<void(std::shared_ptr<ISerializable>)> callback; // Callback when the task is complete

  // Constructor for creating a Task with the given file data and callback
  Task(const std::shared_ptr<ISerializable>& fileData,
       const std::function<void(std::shared_ptr<ISerializable>)>& callback)
  : fileData(fileData), callback(callback) {}
};
