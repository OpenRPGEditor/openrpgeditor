#pragma once

#include "Database/Actors.hpp"
#include "Database/Serializable/ISerializable.hpp"

class ActorsSerializer final : public ITypedSerializable<Actors> {
public:
  explicit ActorsSerializer(const std::string_view filepath)
  : ITypedSerializable(filepath) {}
  ActorsSerializer(const Actors& actors, const std::string_view filepath)
  : ITypedSerializable(actors, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
