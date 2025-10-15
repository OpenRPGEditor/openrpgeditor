#include "Core/LCF_Importer/LCF_Mapping.hpp"

#include "Database/Database.hpp"

#include <fstream>
void to_json(nlohmann::ordered_json& json, const LCF_Mapping& mapping) {
  json = {
      {"switch_mapping", mapping.switch_mapping}, {"variable_mapping", mapping.variable_mapping}, {"common_mapping", mapping.common_mapping}, {"actor_mapping", mapping.actor_mapping},
      {"state_mapping", mapping.state_mapping},   {"image_mapping", mapping.image_mapping},       {"sound_mapping", mapping.sound_mapping},   {"characterName_mapping", mapping.characterName_mapping},
  };
}
void from_json(const nlohmann::ordered_json& json, LCF_Mapping& mapping) {
  mapping.switch_mapping = json.value("switch_mapping", mapping.switch_mapping);
  mapping.variable_mapping = json.value("variable_mapping", mapping.variable_mapping);
  mapping.common_mapping = json.value("common_mapping", mapping.common_mapping);
  mapping.actor_mapping = json.value("actor_mapping", mapping.actor_mapping);
  mapping.state_mapping = json.value("state_mapping", mapping.state_mapping);
  mapping.image_mapping = json.value("image_mapping", mapping.image_mapping);
  mapping.sound_mapping = json.value("sound_mapping", mapping.sound_mapping);
  mapping.characterName_mapping = json.value("characterName_mapping", mapping.characterName_mapping);
}

LCF_Mapping LCF_Mapping::load(const std::filesystem::path& path) {
  if (std::ifstream file(path); file.is_open()) {
    try {
      nlohmann::json data = nlohmann::ordered_json::parse(file);
      LCF_Mapping ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool LCF_Mapping::serialize(const std::filesystem::path& path) {
  std::ofstream file(path);
  try {
    if (file.is_open()) {
      const nlohmann::ordered_json data = *this;
      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}

bool LCF_Mapping::hasUnresolvedPairs() {
  bool result{false};
  for (auto& val : switch_mapping) {
    if (val.second == 0) {
      result = true;
    }
  }
  for (auto& val : variable_mapping) {
    if (val.second == 0) {
      result = true;
    }
  }
  for (auto& val : common_mapping) {
    if (val.second == 0) {
      result = true;
    }
  }
  for (auto& val : actor_mapping) {
    if (val.second == 0) {
      result = true;
    }
  }
  for (auto& val : state_mapping) {
    if (val.second == 0) {
      result = true;
    }
  }
  for (auto& val : sound_mapping) {
    if (val.second.empty()) {
      result = true;
    }
  }
  for (auto& val : image_mapping) {
    if (val.second.empty()) {
      result = true;
    }
  }
  for (auto& val : characterName_mapping) {
    if (val.second.empty()) {
      result = true;
    }
  }
  m_hasUnresolved = result;
  return result;
}
bool LCF_Mapping::isUnresolved() { return m_hasUnresolved; }
void LCF_Mapping::loadDatabase(const lcf::rpg::Database* database) {
  for (auto& val : database->switches) {
    if (!val.name.empty() && !switch_mapping.contains(val.ID)) {
      switch_mapping.insert(std::make_pair(val.ID, 0));
    }
  }
  for (auto& val : database->variables) {
    if (!val.name.empty() && !variable_mapping.contains(val.ID)) {
      variable_mapping.insert(std::make_pair(val.ID, 0));
    }
  }
  for (auto& val : database->commonevents) {
    if (!val.name.empty() && !common_mapping.contains(val.ID)) {
      common_mapping.insert(std::make_pair(val.ID, 0));
    }
  }
  for (auto& val : database->actors) {
    if (!val.name.empty() && !actor_mapping.contains(val.ID)) {
      actor_mapping.insert(std::make_pair(val.ID, 0));
    }
  }
  for (auto& val : database->states) {
    if (!val.name.empty() && !state_mapping.contains(val.ID)) {
      state_mapping.insert(std::make_pair(val.ID, 0));
    }
  }
}
void LCF_Mapping::addEmptySwitch(int id) {
  switch_mapping.insert(std::make_pair(id, 0));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyVariable(int id) {
  variable_mapping.insert(std::make_pair(id, 0));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyCommonEvent(int id) {
  common_mapping.insert(std::make_pair(id, 0));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyActor(int id) {
  actor_mapping.insert(std::make_pair(id, 0));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyState(int id) {
  state_mapping.insert(std::make_pair(id, 0));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptySound(std::string& name) {
  sound_mapping.insert(std::make_pair(name, ""));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyImage(std::string& name) {
  image_mapping.insert(std::make_pair(name, ""));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyCharacterName(std::string& name) {
  characterName_mapping.insert(std::make_pair(name, ""));
  m_hasUnresolved = true;
}
std::string LCF_Mapping::replaceText(std::string& text) {
  if (text.contains("N[0]")) {
    text.replace(text.find("N[0]"), 4, "V[111]");
  }
  if (text.contains("N[25]")) {
    text.replace(text.find("N[25]"), 5, "V[296]");
  }
  if (text.contains("N[26]")) {
    text.replace(text.find("N[26]"), 5, "V[297]");
  }
  if (text.contains("N[27]")) {
    text.replace(text.find("N[27]"), 5, "V[298]");
  }
  if (text.contains("N[28]")) {
    text.replace(text.find("N[28]"), 5, "V[299]");
  }
  if (text.contains("　")) {
    text.replace(text.find("　"), 1, " ");
  }
  if (text.contains("（")) {
    text.replace(text.find("（"), 1, "(");
  }
  if (text.contains("）")) {
    text.replace(text.find("）"), 1, ")");
  }
  if (text.contains("$g")) {
    text.replace(text.find("$g"), 2, "♡");
  }
  if (text.contains("$k")) {
    text.replace(text.find("$k"), 2, "♥");
  }
  if (text.contains("$v")) {
    text.replace(text.find("$v"), 2, "☜");
  }
  // if (text.contains("♪")) {
  //   text.replace(text.find("♪"), 1, "♪");
  // }
  if (text.contains("$t")) {
    text.replace(text.find("$t"), 2, "☞");
  }
  if (text.contains("$e")) {
    text.replace(text.find("$e"), 2, "");
  }
  if (text.contains("$d")) {
    text.replace(text.find("$e"), 2, "");
  }
  if (text.contains("C[11]")) {
    text.replace(text.find("C[11]"), 5, "C[18]");
  }
  if (text.contains("C[15]")) {
    text.replace(text.find("C[15]"), 5, "C[24]");
  }
  if (text.contains("C[19]")) {
    text.replace(text.find("C[19]"), 5, "C[20]");
  }
  if (text.contains("C[17]")) {
    text.replace(text.find("C[17]"), 5, "C[36]");
  }
  if (text.contains("C[13]")) {
    text.replace(text.find("C[13]"), 5, "C[30]");
  }
  // if (text.contains("～")) {
  //   text.replace(text.find("～"), 1, "~");
  // }
  auto pos1 = text.find("「");
  auto pos2 = text.find("」");
  if (pos1 != std::string::npos && pos2 != std::string::npos) {
    std::string prefix = text.substr(0, pos1 + 3);
    std::string suffix = text.substr(pos2);
    std::string substring = text.substr(pos1 + 3, text.size() - (prefix.size() + suffix.size()));

    return "";
  }
  auto pos3 = text.find('(');
  auto pos4 = text.find(')');

  if (pos3 != std::string::npos && pos4 != std::string::npos) {
    std::string prefix = text.substr(0, pos3);
    std::string substring = text.substr(pos3, text.size() - prefix.size());

    return prefix + substring;
  }
  return text;
}

int LCF_Mapping::switchValue(int key) {
  if (switch_mapping.empty() || !switch_mapping.contains(key)) {
    addEmptySwitch(key);
    return 5000;
  }
  int ret = switch_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return 5000;
  }
  return ret;
}
int LCF_Mapping::variableValue(int key) {
  if (variable_mapping.empty() || !variable_mapping.contains(key)) {
    addEmptyVariable(key);
    return 5000;
  }
  int ret = variable_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return 5000;
  }
  return ret;
}
int LCF_Mapping::commonEventValue(int key) {
  if (common_mapping.empty() || !common_mapping.contains(key)) {
    addEmptyCommonEvent(key);
    return 1;
  }
  int ret = common_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return 1;
  }
  return ret;
}
int LCF_Mapping::actorValue(int key) {
  if (actor_mapping.empty() || !actor_mapping.contains(key)) {
    addEmptyCommonEvent(key);
    return key;
  }
  int ret = actor_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return key;
  }
  return ret;
}
int LCF_Mapping::stateValue(int key) {
  if (state_mapping.empty() || !state_mapping.contains(key)) {
    addEmptyCommonEvent(key);
    return key;
  }
  int ret = state_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return key;
  }
  return ret;
}
std::string LCF_Mapping::localeValue(std::string& text, int mapId, int evId, int page, int command) { return "{}"; }
std::string LCF_Mapping::soundValue(std::string key) {
  if (sound_mapping.empty() || !sound_mapping.contains(key)) {
    addEmptySound(key);
    return key;
  }
  std::string ret = sound_mapping.at(key);
  if (ret.empty()) {
    m_hasUnresolved = true;
    return key;
  }
  return ret;
}
std::string LCF_Mapping::imageValue(std::string& key) {
  if (image_mapping.empty() || !image_mapping.contains(key)) {
    addEmptyImage(key);
    return key;
  }
  std::string ret = image_mapping.at(key);
  if (ret.empty()) {
    m_hasUnresolved = true;
    return ret;
  }
  return ret;
}
std::string LCF_Mapping::characterNameValue(std::string& key) {
  if (key.empty()) {
    return key;
  }
  if (characterName_mapping.empty() || !characterName_mapping.contains(key)) {
    addEmptyCharacterName(key);
    return key;
  }
  std::string ret = characterName_mapping.at(key);
  if (ret.empty()) {
    m_hasUnresolved = true;
    return ret;
  }
  return ret;
}