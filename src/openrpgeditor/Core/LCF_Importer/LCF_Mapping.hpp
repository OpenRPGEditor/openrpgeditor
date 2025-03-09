#pragma once

#include "Core/LCF_Importer/LCF_Mapping.hpp"
#include <nlohmann/json.hpp>

#include <filesystem>
#include <map>
#include <memory>

class LCF_Mapping {
public:
  static LCF_Mapping load(std::string_view path);
  bool serialize(std::string_view path);
  void save(std::string path) {
    serialize(path.append("editor/LCFMapping.json")); // Make new file if it doesn't exist
  }

  void addEmptySwitch(int id);
  void addEmptyVariable(int id);
  void addEmptyAnimation(int id);
  void addEmptySound(std::string name);
  void addEmptyImage(std::string name);

  int switchValue(int key);
  int variableValue(int key);
  int animationValue(int key);

  std::string soundValue(std::string key);
  std::string imageValue(std::string key);

  bool hasUnresolvedPairs();
  bool isUnresolved();

  std::map<int, int> switch_mapping;
  std::map<int, int> variable_mapping;
  std::map<int, int> animation_mapping;
  std::map<std::string, std::string> image_mapping;
  std::map<std::string, std::string> sound_mapping;

private:
  bool m_hasUnresolved{false};
};

void to_json(nlohmann::ordered_json& j, const LCF_Mapping& mapper);
void from_json(const nlohmann::ordered_json& j, LCF_Mapping& mapper);
