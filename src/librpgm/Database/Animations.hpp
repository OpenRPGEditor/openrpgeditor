#pragma once
#include <string_view>

#include "Database/Animation.hpp"

class Animations {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE(Animations, m_animations);
  Animations();

  static Animations load(std::string_view filename);
  void serialize(std::string_view filename);

  // private:
  std::vector<Animation> m_animations;
};
