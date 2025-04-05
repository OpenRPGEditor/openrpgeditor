#pragma once
#include "Database/MapInfos.hpp"
#include "Database/Serializable/ISerializable.hpp"

class MapInfosSerializer final : public ISerializable {
public:
  explicit MapInfosSerializer(const std::string_view filepath)
  : ISerializable{filepath} {}
  MapInfosSerializer(MapInfos mapInfos, const std::string_view filepath)
  : ISerializable{filepath}
  , m_data(mapInfos) {}

  void serialize(std::ofstream& outFile) const override;
  void deserialize(std::ifstream& inFile) override;

  MapInfos data() const { return m_data; }

private:
  MapInfos m_data;
};
