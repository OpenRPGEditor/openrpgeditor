#include "Core/LCF_Importer/LCF_Mapping.hpp"
#include <fstream>
void to_json(nlohmann::ordered_json& json, const LCF_Mapping& mapping) {
  json = {
      {"switch_mapping", mapping.switch_mapping}, {"variable_mapping", mapping.variable_mapping}, {"animation_mapping", mapping.animation_mapping},
      {"image_mapping", mapping.image_mapping},   {"sound_mapping", mapping.sound_mapping},
  };
}
void from_json(const nlohmann::ordered_json& json, LCF_Mapping& mapping) {
  mapping.switch_mapping = json.value("switch_mapping", mapping.switch_mapping);
  mapping.variable_mapping = json.value("variable_mapping", mapping.variable_mapping);
  mapping.animation_mapping = json.value("animation_mapping", mapping.animation_mapping);
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
  for (auto& val : animation_mapping) {
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
void LCF_Mapping::addEmptyAnimation(int id) {
  animation_mapping.insert(std::make_pair(id, 0));
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
    return 1;
  }
  int ret = switch_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return 1;
  }
  return ret;
}
int LCF_Mapping::variableValue(int key) {
  if (variable_mapping.empty() || !variable_mapping.contains(key)) {
    addEmptyVariable(key);
    return 1;
  }
  int ret = variable_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return 1;
  }
  return ret;
}
int LCF_Mapping::animationValue(int key) {
  if (animation_mapping.empty() || !animation_mapping.contains(key)) {
    addEmptyAnimation(key);
    return 1;
  }
  int ret = animation_mapping.at(key);
  if (ret == 0) {
    m_hasUnresolved = true;
    return 1;
  }
  return ret;
}
std::string LCF_Mapping::soundValue(std::string key) {
  if (sound_mapping.empty() || !sound_mapping.contains(key)) {
    addEmptySound(key);
    return "";
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
    return "";
  }
  std::string ret = image_mapping.at(key);
  if (ret.empty()) {
    m_hasUnresolved = true;
    return ret;
  }
  return ret;
}