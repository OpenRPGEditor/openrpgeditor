#pragma once
#include "Database/Serializable/ISerializable.hpp"
#include "Database/States.hpp"

class StatesSerializer final : public ITypedSerializable<States> {
public:
  explicit StatesSerializer(const std::string_view filepath)
  : ITypedSerializable(filepath) {}
  StatesSerializer(const States& states, const std::string_view filepath)
  : ITypedSerializable(states, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
