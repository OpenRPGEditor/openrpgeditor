#pragma once

#include "Database/Battler.hpp"
#include "Database/Globals.hpp"
#include "Database/Terms.hpp"
#include "Database/Vehicle.hpp"

#include <nlohmann/json.hpp>

class System {
  friend void to_json(nlohmann::ordered_json& j, const System& system);
  friend void from_json(const nlohmann::ordered_json& j, System& system);

public:
  struct Motion {
    friend void to_json(nlohmann::ordered_json& j, const Motion& motion);
    friend void from_json(const nlohmann::ordered_json& j, Motion& motion);
    int type = 0;
    int weaponImageId = 0;
  };

  Vehicle airship;
  std::vector<std::string> armorTypes;
  std::vector<Motion> attackMotions;
  Audio battleBgm;
  std::string battleback1Name;
  std::string battleback2Name;
  int battlerHue{};
  std::string battlerName;
  Vehicle boat;
  std::string currencyUnit;
  Audio defeatMe;
  int editMapId{};
  std::vector<std::string> elements;
  std::vector<std::string> equipTypes;
  std::string gameTitle;
  Audio gameoverMe;
  std::string locale;
  std::vector<int> magicSkills;
  std::array<bool, 6> menuCommands{}; // Item, Skill, Equip, Status, Formation, Save;
  bool optDisplayTp{};
  bool optDrawTitle{};
  bool optExtraExp{};
  bool optFloorDeath{};
  bool optFollowers{};
  bool optSideView{};
  bool optSlipDeath{};
  bool optTransparent{};
  std::vector<int> partyMembers;
  Vehicle ship;
  std::vector<std::string> skillTypes;
  std::vector<Audio> sounds;
  int startMapId{};
  int startX{};
  int startY{};
  std::vector<std::string> switches;
  Terms terms;
  std::vector<Battler> testBattlers;
  int testTroopId{};
  std::string title1Name;
  std::string title2Name;
  Audio titleBgm;
  std::vector<std::string> variables;
  int tileSize{48};
  uint32_t versionId = 0;
  Audio victoryMe;
  std::vector<std::string> weaponTypes;
  std::vector<int> windowTone;
  std::optional<bool> hasEncryptedImages;
  std::optional<bool> hasEncryptedAudio;
  std::optional<std::string> encryptionKey;

  static System load(std::string_view filepath);
  bool serialize(std::string_view filename);

  [[nodiscard]] std::string& variable(const int id) { return variables[id]; }
  [[nodiscard]] const std::string& variable(const int id) const { return variables[id]; }

  [[nodiscard]] std::string& switche(const int id) { return switches[id]; }
  [[nodiscard]] const std::string& switche(const int id) const { return switches[id]; }

  [[nodiscard]] std::string* element(const int id) {
    if (id < 0 || id >= elements.size()) {
      return nullptr;
    }
    return &elements[id];
  }

  [[nodiscard]] const std::string* element(const int id) const {
    if (id < 0 || id >= elements.size()) {
      return nullptr;
    }
    return &elements[id];
  }

  [[nodiscard]] std::string* skillType(const int id) {
    if (id < 0 || id >= skillTypes.size()) {
      return nullptr;
    }
    return &skillTypes[id];
  }

  [[nodiscard]] const std::string* skillType(const int id) const {
    if (id < 0 || id >= skillTypes.size()) {
      return nullptr;
    }
    return &skillTypes[id];
  }

  [[nodiscard]] std::string* equipType(const int id) {
    if (id < 0 || id >= equipTypes.size()) {
      return nullptr;
    }
    return &equipTypes[id];
  }

  [[nodiscard]] const std::string* equipType(const int id) const {
    if (id < 0 || id >= equipTypes.size()) {
      return nullptr;
    }
    return &equipTypes[id];
  }

  [[nodiscard]] std::string* armorType(const int id) {
    if (id < 0 || id >= armorTypes.size()) {
      return nullptr;
    }
    return &armorTypes[id];
  }

  [[nodiscard]] const std::string* armorType(int id) const {
    if (id < 0 || id >= armorTypes.size()) {
      return nullptr;
    }
    return &armorTypes[id];
  }

  [[nodiscard]] std::string* weaponType(const int id) {
    if (id < 0 || id >= weaponTypes.size()) {
      return nullptr;
    }
    return &weaponTypes[id];
  }

  [[nodiscard]] const std::string* weaponType(const int id) const {
    if (id < 0 || id >= weaponTypes.size()) {
      return nullptr;
    }
    return &weaponTypes[id];
  }

  [[nodiscard]] std::vector<char> encryptionKeyAsBytes() const {
    if (!encryptionKey) {
      return {};
    }

    std::vector<char> bytes;

    for (unsigned int i = 0; i < encryptionKey->length(); i += 2) {
      std::string byteString = encryptionKey->substr(i, 2);
      char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
      bytes.push_back(byte);
    }

    return bytes;
  }
};

void to_json(nlohmann::ordered_json& j, const System::Motion& motion);
void from_json(const nlohmann::ordered_json& j, System::Motion& motion);
void to_json(nlohmann::ordered_json& j, const System& system);
void from_json(const nlohmann::ordered_json& j, System& system);