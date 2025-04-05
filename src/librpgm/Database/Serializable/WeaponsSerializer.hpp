#pragma once
#include "Database/Serializable/ISerializable.hpp"
#include "Database/Weapons.hpp"

class WeaponsSerializer final : public ITypedSerializable<Weapons> {
public:
  explicit WeaponsSerializer(const std::string_view filepath)
  : ITypedSerializable(filepath) {}
  WeaponsSerializer(const Weapons& weapons, const std::string_view filepath)
  : ITypedSerializable(weapons, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
