#include "Core/LCF_Importer/LCF_Mapping.hpp"
#include <fstream>
void to_json(nlohmann::ordered_json& json, const LCF_Mapping& mapping) {
  json = {
      {"switch_mapping", mapping.switch_mapping}, {"variable_mapping", mapping.variable_mapping}, {"common_mapping", mapping.common_mapping}, {"actor_mapping", mapping.actor_mapping},
      {"state_mapping", mapping.state_mapping},   {"image_mapping", mapping.image_mapping},       {"sound_mapping", mapping.sound_mapping},
  };
}
void from_json(const nlohmann::ordered_json& json, LCF_Mapping& mapping) {
  mapping.switch_mapping = json.value("switch_mapping", mapping.switch_mapping);
  mapping.variable_mapping = json.value("variable_mapping", mapping.variable_mapping);
  mapping.common_mapping = json.value("animation_mapping", mapping.common_mapping);
  mapping.actor_mapping = json.value("actor_mapping", mapping.actor_mapping);
  mapping.state_mapping = json.value("state_mapping", mapping.state_mapping);
  mapping.image_mapping = json.value("image_mapping", mapping.image_mapping);
  mapping.sound_mapping = json.value("sound_mapping", mapping.sound_mapping);
}

LCF_Mapping LCF_Mapping::load(std::string_view path) {
  if (std::ifstream file(path.data()); file.is_open()) {
    try {
      nlohmann::json data = nlohmann::ordered_json::parse(file);
      LCF_Mapping ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool LCF_Mapping::serialize(const std::string_view path) {
  std::ofstream file(path.data());
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
  m_hasUnresolved = result;
  return result;
}
bool LCF_Mapping::isUnresolved() { return m_hasUnresolved; };
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
void LCF_Mapping::addEmptySound(std::string name) {
  image_mapping.insert(std::make_pair(name, ""));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyImage(std::string name) {
  sound_mapping.insert(std::make_pair(name, ""));
  m_hasUnresolved = true;
}
int LCF_Mapping::switchValue(int key) {
  if (switch_mapping.empty() || !switch_mapping.contains(key)) {
    addEmptySwitch(key);
    return key;
  }
  int ret = switch_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return key;
  }
  return ret;
}
int LCF_Mapping::variableValue(int key) {
  if (variable_mapping.empty() || !variable_mapping.contains(key)) {
    addEmptyVariable(key);
    return key;
  }
  int ret = variable_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return key;
  }
  return ret;
}
int LCF_Mapping::commonEventValue(int key) {
  if (common_mapping.empty() || !common_mapping.contains(key)) {
    addEmptyCommonEvent(key);
    return key;
  }
  int ret = common_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return key;
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
std::string LCF_Mapping::soundValue(std::string key) {
  if (sound_mapping.empty() || !sound_mapping.contains(key)) {
    addEmptySound(key);
    return key;
  }
  std::string ret = sound_mapping.at(key);
  if (ret.empty()) {
    m_hasUnresolved = true;
    return ret;
  }
  return ret;
}
std::string LCF_Mapping::imageValue(std::string key) {
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