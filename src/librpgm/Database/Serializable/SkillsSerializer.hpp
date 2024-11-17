#pragma once

#include "Database/Serializable/ISerializable.hpp"
#include "Database/Skills.hpp"

class SkillsSerializer final : public ITypedSerializable<Skills> {
public:
  explicit SkillsSerializer(const std::string_view filepath) : ITypedSerializable(filepath) {}
  SkillsSerializer(const Skills& actors, const std::string_view filepath) : ITypedSerializable(actors, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
