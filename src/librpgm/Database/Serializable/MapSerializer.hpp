#pragma once

#include "Database/Map.hpp"
#include "Database/Serializable/ISerializable.hpp"

class MapSerializer final : public ITypedSerializable<Map> {
public:
  explicit MapSerializer(const std::string_view filename, const int mapId)
  : ITypedSerializable(filename)
  , m_mapId(mapId) {}
  MapSerializer(const Map& map, const int mapId, const std::string_view filename)
  : ITypedSerializable(map, filename)
  , m_mapId(mapId) {}

  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;

  [[nodiscard]] int mapId() const { return m_mapId; }

private:
  int m_mapId = 0;
};
