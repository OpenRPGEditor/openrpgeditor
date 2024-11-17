#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/System.hpp"

class SystemSerializer final : public ITypedSerializable<System> {
public:
  explicit SystemSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  SystemSerializer(const System& system, const std::string_view filepath) : ITypedSerializable(system, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
