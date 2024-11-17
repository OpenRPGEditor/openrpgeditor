#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/Tilesets.hpp"

class TilesetsSerializer final : public ITypedSerializable<Tilesets> {
public:
  explicit TilesetsSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  TilesetsSerializer(const Tilesets& tilesets, const std::string_view filepath) : ITypedSerializable(tilesets, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
