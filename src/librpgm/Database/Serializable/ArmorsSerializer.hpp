#pragma once
#include "Database/Serializable/ISerializable.hpp"
#include "Database/Armors.hpp"

class ArmorsSerializer final : public ITypedSerializable<Armors> {
public:
  explicit ArmorsSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  ArmorsSerializer(const Armors& armors, const std::string_view filepath) : ITypedSerializable(armors, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
