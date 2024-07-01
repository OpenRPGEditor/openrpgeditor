#pragma once

#include <map>
#include <string>
#include <nlohmann/json.hpp>

struct GameConstants {
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(GameConstants, variables, switches, actors, classes, skills, items,
                                              weapons, armors, enemies, troops, states, animations, tilesets,
                                              commonEvents, maps, generateJS);

  enum class Type {
    Variable,
    Switch,
    Actor,
    Class,
    Skill,
    Item,
    Weapon,
    Armor,
    Enemy,
    Troop,
    State,
    Animation,
    Tileset,
    CommonEvent,
    Map
  };
  std::map<int, std::string> variables;
  std::map<int, std::string> switches;
  std::map<int, std::string> actors;
  std::map<int, std::string> classes;
  std::map<int, std::string> skills;
  std::map<int, std::string> items;
  std::map<int, std::string> weapons;
  std::map<int, std::string> armors;
  std::map<int, std::string> enemies;
  std::map<int, std::string> troops;
  std::map<int, std::string> states;
  std::map<int, std::string> animations;
  std::map<int, std::string> tilesets;
  std::map<int, std::string> commonEvents;
  std::map<int, std::string> maps;
  bool generateJS{false};

  static GameConstants load(std::string_view path);
  bool serialize(std::string_view path);

  bool generateConstantsJS(std::string_view path);

  bool isValidName(Type type, int id, const std::string& constant);
};
