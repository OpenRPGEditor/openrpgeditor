#pragma once

#include <functional>
#include <memory>

// Forward declaration of ISerializable, which represents a file data object.
class ISerializable;

// Task metadata and associated callback
class Task {
public:
  Task() = default;
  Task(const Task&) = default;
  Task& operator=(const Task&) = default;
  Task(Task&&) = default;
  Task& operator=(Task&&) = default;

  Task(const std::shared_ptr<ISerializable>& fileData, const std::function<void(std::shared_ptr<ISerializable>)>& callback)
  : m_fileData(fileData)
  , m_callback(callback) {}

  std::string_view filepath() const { return m_fileData->filepath(); }
  std::shared_ptr<ISerializable> fileData() const { return m_fileData; }
  ISerializable::Operation operation() const { return m_fileData->operation(); }
  std::function<void(std::shared_ptr<ISerializable>)> callback() const { return m_callback; }

private:
  std::shared_ptr<ISerializable> m_fileData;                      // The file data (serializable object)
  std::function<void(std::shared_ptr<ISerializable>)> m_callback; // Callback when the task is complete
};
