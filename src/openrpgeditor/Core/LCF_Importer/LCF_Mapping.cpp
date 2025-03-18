#include "Core/LCF_Importer/LCF_Mapping.hpp"

#include "Database/Database.hpp"

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
  image_mapping.insert(std::make_pair(name, ""));
  m_hasUnresolved = true;
}
void LCF_Mapping::addEmptyImage(std::string& name) {
  sound_mapping.insert(std::make_pair(name, ""));
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

    std::string finalText = prefix + removeSurrogators(substring) + suffix;
    finalText = checkParenthesis(finalText);
    return finalText;
  }
  auto pos3 = text.find('(');
  auto pos4 = text.find(')');

  if (pos3 != std::string::npos && pos4 != std::string::npos) {
    std::string prefix = text.substr(0, pos3);
    std::string substring = text.substr(pos3, text.size() - prefix.size());

    substring = removeSurrogators(substring);

    return prefix + substring;
  }
  return text;
}

std::string LCF_Mapping::checkParenthesis(std::string& text) {
  auto pos3 = text.find('(');
  auto pos4 = text.find(')');

  if (pos3 != std::string::npos && pos4 != std::string::npos) {
    std::string prefix = text.substr(0, pos3);
    // std::string suffix = text.substr(text.size());
    std::string substring = text.substr(pos3, text.size() - prefix.size());

    substring = removeSurrogators(substring);

    return prefix + substring;
  }
  return text;
}

std::string LCF_Mapping::removeSurrogators(std::string& substring) {

  while (substring.find("―") != std::string::npos) {
    substring = substring.replace(substring.find("―"), 1, "");
  }
  while (substring.find("　") != std::string::npos) {
    substring = substring.replace(substring.find("　"), 1, "");
  }
  while (substring.find("～") != std::string::npos) {
    substring = substring.replace(substring.find("～"), 1, "~");
  }
  for (auto& chr : substring) {
    if (static_cast<int>(chr) < 0) {
      if (static_cast<int>(chr) == -29) {
        substring = substring.replace(substring.find(chr), 1, " ");
      } else if (static_cast<int>(chr) == -86 || static_cast<int>(chr) == -103 || static_cast<int>(chr) == -30) {
        // Music note
      } else {
        substring = substring.replace(substring.find(chr), 1, "`");
      }
    }
  }
  while (substring.find('`') != std::string::npos) {
    substring = substring.replace(substring.find('`'), 1, "");
  }
  return substring;
};

bool LCF_Mapping::checkAllCharacters(std::string& text1, std::string text2) {
  int count{0};
  bool resultInvalid{false};
  if (text1.size() == text2.size()) {
    for (int i = 0; i < text1.size() - 1; i++) {
      if (text1.at(i) != text2.at(i)) {
        resultInvalid = true;
      }
      if (!resultInvalid) {
        count++;
      }
    }
  } else {
    resultInvalid = true;
  }
  if (count >= text1.size() - 5 && count > 0) {

    if (count == text1.size() - 1) {
      // Count exact
      int test = 2;
    } else {
      int test = 2;
      // Some missing
    }
  }

  if (resultInvalid) {
    return false;
  }
  return true;
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
int LCF_Mapping::findTextMatch(std::string text) {

  int highestMatch{0};
  int highestIndex{-1};
  int pairIndex{0};
  for (auto& pair : Database::instance()->locales.locales) {
    int count{0};
    bool isMismatch{false}; // Boolean to check if the current iteration is a match or not. This will be enabled if it fails so it doesn't continue unnecessarily processing.
    // 1. Iterate through text and compare it to the current locale pair
    // if (text.size() == pair.second.size()) {
    //
    //}
    if (text.size() <= pair.second.size()) {
      for (int i = 0; i < text.size(); i++) {
        if (!isMismatch) {
          if (text.at(i) != pair.second.at(i)) {
            isMismatch = true;
            highestMatch = count;
            highestIndex = pairIndex;
          }
          count++;
          if (count > highestMatch) {
            highestMatch = count;
            highestIndex = pairIndex;
          }
        }
      }
      keyCount++;
      if (highestMatch == text.size()) {
        return highestIndex;
      }
    }
    pairIndex++; // The index of the pair we're comparing. Increment for every end of pair until the end of processing.
  }
  if ((text.size() - 1) - highestMatch > 3) {
    return -1; // If the difference is bigger than 5 characters, then it's likely not the match. (Going to reduce this to 2 or 3)
  }
  return highestIndex; // Return the highest index if the comparison is close enough or equal to
}
std::string LCF_Mapping::localeValue(std::string& text, int mapId, int evId, int page, int command) {

  return "{}";

  text = replaceText(text);
  int key = findTextMatch(text);
  if (key != -1) {
    if (mapId == -1) {
      return Database::instance()->locales.locales.at(key).first;
    }
    std::string pair = Database::instance()->locales.locales.at(key).first;
    if (!pair.contains('{')) {
      std::string newKey = "{" + std::format("Map{}-EV{}-Page{}-{}", mapId, evId, page, command) + "}";
      Database::instance()->locales.locales.at(key).first = newKey;
      return newKey; // Result found
    }
    return Database::instance()->locales.locales.at(key).first;
  }
  return "Verify key";

  // std::string compareString;
  // for (auto& pair : Database::instance()->locales.locales) {
  //   compareString = pair.second;
  //   bool isMatch = checkAllCharacters(text, pair.second);
  //   if (isMatch) {
  //
  //     // Don't replace key if it already is formatted -- just use it.
  //     return pair.first;
  //   }
  // }
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