#pragma once

#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>

#include <vector>
#include <string>
#include <array>

struct Learning {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Learning, level, note, skillId);

  int level{};
  std::string note;
  int skillId{};
};

struct Class {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Class, id, expParams, traits, learnings, name, note, params);
  int id{};
  std::array<int, 4> expParams{}; // Base, Extra, Acceleration A, Acceleration B
  std::vector<Trait> traits;
  std::vector<Learning> learnings;
  std::string name;
  std::string note;
  std::array<std::vector<int>, 8> params;
};

class Classes {
public:
  static Classes load(std::string_view filename);
  void serialize(std::string_view filename);

  std::vector<Class> m_classes;
};
