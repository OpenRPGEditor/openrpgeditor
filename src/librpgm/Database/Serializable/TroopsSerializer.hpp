#pragma once
#include "Database/Serializable/ISerializable.hpp"
#include "Database/Troops.hpp"

class TroopsSerializer final : public ITypedSerializable<Troops> {
public:
  explicit TroopsSerializer(const std::string_view filepath)
  : ITypedSerializable(filepath) {}
  TroopsSerializer(const Troops& troops, const std::string_view filepath)
  : ITypedSerializable(troops, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
