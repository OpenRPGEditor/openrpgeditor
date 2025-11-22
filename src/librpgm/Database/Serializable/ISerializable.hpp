#pragma once

#include <fstream>
#include <string>

class ISerializable {
public:
  enum class Operation {
    Read,
    Write,
  };
  explicit ISerializable(const std::string_view filepath, const Operation operation)
  : m_filepath(filepath)
  , m_operation(operation) {}

  virtual ~ISerializable() = default;

  // Virtual methods for serialization and deserialization
  virtual void serialize(std::ofstream& outFile) const = 0; // Serialize the object to an output stream
  virtual void deserialize(std::ifstream& inFile) = 0;      // Deserialize the object from an input stream

  // Getter for the file path associated with this object
  [[nodiscard]] virtual std::string_view filepath() const { return m_filepath; }

  [[nodiscard]] Operation operation() const { return m_operation; }

private:
  std::string m_filepath;
  Operation m_operation;
};

template <typename T>
class ITypedSerializable : public ISerializable {
public:
  explicit ITypedSerializable(const std::string_view filepath)
  : ISerializable(filepath, Operation::Read) {}
  ITypedSerializable(const T& data, const std::string_view filepath)
  : ISerializable(filepath, Operation::Write)
  , m_data(data) {}

  const T& data() const { return m_data; }

protected:
  T m_data;
};