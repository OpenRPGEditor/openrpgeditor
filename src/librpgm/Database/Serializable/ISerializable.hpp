#pragma once

#include <fstream>
#include <string>

class ISerializable {
public:
  explicit ISerializable(const std::string_view filepath)
  : m_filepath(filepath) {}
  virtual ~ISerializable() = default;

  // Virtual methods for serialization and deserialization
  virtual void serialize(std::ofstream& outFile) const = 0; // Serialize the object to an output stream
  virtual void deserialize(std::ifstream& inFile) = 0;      // Deserialize the object from an input stream

  // Getter for the file path associated with this object
  [[nodiscard]] virtual std::string_view filepath() const { return m_filepath; }

private:
  std::string m_filepath;
};

template <typename T>
class ITypedSerializable : public ISerializable {
public:
  explicit ITypedSerializable(const std::string_view filepath)
  : ISerializable(filepath) {}
  ITypedSerializable(const T& data, const std::string_view filepath)
  : ISerializable(filepath)
  , m_data(data) {}
  const T& data() const { return m_data; }

protected:
  T m_data;
};