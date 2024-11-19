#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/Map.hpp"

class MapSerializer final : public ITypedSerializable<Map> {
public:
  explicit MapSerializer(const std::string_view filename, int mapId) : ITypedSerializable(filename), m_mapId(mapId) {}
  MapSerializer(const Map& map, const std::string_view filename) : ITypedSerializable(map, filename) {}

  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;

  [[nodiscard]] int mapId() const {return m_mapId; }
private:
  int m_mapId = 0;
};
