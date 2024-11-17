#pragma once

#include <string>
#include <fstream>

class ISerializable {
public:
  explicit ISerializable(const std::string_view filepath) : m_filepath(filepath) {}
  virtual ~ISerializable() = default;

  // Virtual methods for serialization and deserialization
  virtual void serialize(std::ofstream& outFile) const = 0; // Serialize the object to an output stream
  virtual void deserialize(std::ifstream& inFile) = 0;      // Deserialize the object from an input stream

  // Getter for the file path associated with this object
  [[nodiscard]] virtual std::string_view filepath() const { return m_filepath; }

protected:
  [[nodiscard]] virtual const void* dataInternal() const = 0;

private:
  std::string m_filepath;
};

template <typename T>
class ITypedSerializable : public ISerializable {
public:
  explicit ITypedSerializable(const std::string_view filepath) : ISerializable(filepath) {}
  const T* data() const { return static_cast<const T*>(dataInternal()); }
};