#pragma once

#include "Editor/LCF_Importer/LCF_Mapping.hpp"
#include "lcf/rpg/database.h"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <map>
#include <memory>

class LCF_Mapping {
public:
  static LCF_Mapping load(const std::filesystem::path& path);
  bool serialize(const std::filesystem::path& path);
  void save(const std::filesystem::path& path) {
    serialize(path / "editor/LCFMapping.json"); // Make new file if it doesn't exist
  }

  void addEmptySwitch(int id);
  void addEmptyVariable(int id);
  void addEmptyCommonEvent(int id);
  void addEmptyActor(int id);
  void addEmptyState(int id);
  void addEmptySound(std::string& name);
  void addEmptyImage(std::string& name);
  void addEmptyCharacterName(std::string& name);

  std::string replaceText(std::string& text);

  int switchValue(int key);
  int variableValue(int key);
  int commonEventValue(int key);
  int actorValue(int key);
  int stateValue(int key);

  std::string localeValue(std::string& text, int mapId, int evId, int page, int command);
  std::string soundValue(std::string key);
  std::string imageValue(std::string& key);
  std::string characterNameValue(std::string& key);

  bool hasUnresolvedPairs();
  bool isUnresolved();
  void loadDatabase(const lcf::rpg::Database* database);

  std::map<int, int> switch_mapping;
  std::map<int, int> variable_mapping;
  std::map<int, int> common_mapping;
  std::map<int, int> actor_mapping;
  std::map<int, int> state_mapping;
  std::map<std::string, std::string> image_mapping;
  std::map<std::string, std::string> sound_mapping;
  std::map<std::string, std::string> characterName_mapping;

  std::map<std::string, std::string> dialogue_errors;
  int keyCount{0};

private:
  bool m_hasUnresolved{false};
};

void to_json(nlohmann::ordered_json& j, const LCF_Mapping& mapper);
void from_json(const nlohmann::ordered_json& j, LCF_Mapping& mapper);
