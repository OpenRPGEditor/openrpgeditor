#pragma once
#include "Database/MapInfos.hpp"
#include "Database/Serializable/ISerializable.hpp"

class MapInfosSerializer final : public ITypedSerializable<MapInfos> {
public:
  explicit MapInfosSerializer(const std::string_view filepath)
  : ITypedSerializable(filepath) {}
  MapInfosSerializer(const MapInfos& mapInfos, const std::string_view filepath)
  : ITypedSerializable(mapInfos, filepath) {}

  void serialize(std::ofstream& outFile) const override;
  void deserialize(std::ifstream& inFile) override;
};
