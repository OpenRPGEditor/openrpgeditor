#pragma once
#include "Database/Animations.hpp"
#include "Database/Serializable/ISerializable.hpp"

class AnimationsSerializer final : public ITypedSerializable<Animations> {
public:
  explicit AnimationsSerializer(const std::string_view filepath)
  : ITypedSerializable(filepath) {}
  AnimationsSerializer(const Animations& animations, const std::string_view filepath)
  : ITypedSerializable(animations, filepath) {}
  void serialize(std::ofstream& os) const override;
  void deserialize(std::ifstream& is) override;
};
