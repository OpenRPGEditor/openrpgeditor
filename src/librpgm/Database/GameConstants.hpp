#pragma once

#include <map>
#include <string>
#include <nlohmann/json.hpp>

struct GameConstants {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(GameConstants, variables, switches, generateJS);

  enum class Type {
    Variable,
    Switch
  };
  std::map<int, std::string> variables;
  std::map<int, std::string> switches;
  bool generateJS{false};

  static GameConstants load(std::string_view path);
  bool serialize(std::string_view path);

  bool generateConstantsJS(std::string_view path);

  bool isValidName(Type type, int id, const std::string& constant);
};
