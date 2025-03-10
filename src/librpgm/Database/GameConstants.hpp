#pragma once

#include <map>
#include <nlohmann/json.hpp>
#include <string>

struct GameConstants {
  friend void to_json(nlohmann::ordered_json& json, const GameConstants& constants);
  friend void from_json(const nlohmann::ordered_json& json, GameConstants& constants);
  enum class Type { Variable, Switch, Actor, Class, Skill, Item, Weapon, Armor, Enemy, Troop, State, Animation, Tileset, CommonEvent, Map };
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

  void swap(int keyOne, int keyTwo, std::map<int, std::string>& keyList) {
    std::string keyOneString = keyList.at(keyOne);
    std::string keyTwoString = keyList.at(keyTwo);
    keyList.at(keyOne) = keyTwoString;
    keyList.at(keyTwo) = keyOneString;
  }
};
void to_json(nlohmann::ordered_json& json, const GameConstants& constants);
void from_json(const nlohmann::ordered_json& json, GameConstants& constants);