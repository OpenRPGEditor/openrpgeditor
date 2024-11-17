#pragma once

#include <string>
#include <fstream>

class ISerializable {
public:
  virtual ~ISerializable() = default;

  // Virtual methods for serialization and deserialization
  virtual void serialize(std::ofstream& outFile) const = 0; // Serialize the object to an output stream
  virtual void deserialize(std::ifstream& inFile) = 0;      // Deserialize the object from an input stream

  // Getter for the file path associated with this object
  [[nodiscard]] virtual std::string_view getFilePath() const = 0;

  // Status methods to indicate whether the object is in a valid state after serialization/deserialization
  [[nodiscard]] virtual bool isValid() const = 0;

  [[nodiscard]] virtual const void* getData() const = 0;
};
