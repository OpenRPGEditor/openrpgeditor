#pragma once

#include "Database/Classes.hpp"
#include "Database/Serializable/ISerializable.hpp"

class ClassesSerializer final : public ITypedSerializable<Classes> {
public:
  explicit ClassesSerializer(const std::string_view filepath)
  : ITypedSerializable(filepath) {}
  ClassesSerializer(const Classes& classes, const std::string_view filepath)
  : ITypedSerializable(classes, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
