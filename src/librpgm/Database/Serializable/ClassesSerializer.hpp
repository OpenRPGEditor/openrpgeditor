#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/Classes.hpp"

class ClassesSerializer final : public ITypedSerializable<Classes> {
public:
  explicit ClassesSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  ClassesSerializer(const Classes& actors, const std::string_view filepath)
  : ITypedSerializable(filepath), m_classes(actors) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;

private:
  [[nodiscard]] const void* dataInternal() const override { return &m_classes; }
  Classes m_classes;
};
