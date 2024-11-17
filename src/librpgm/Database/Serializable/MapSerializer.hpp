#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/Map.hpp"

class MapSerializer : public ITypedSerializable<Map> {
public:
  explicit MapSerializer(const std::string_view filename) : ITypedSerializable(filename) {}

  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;

private:
  [[nodiscard]] const void* dataInternal() const override { return &m_map; }
  Map m_map;
};
