#pragma once

#include "Database/Globals.hpp"
#include "Database/Battler.hpp"
#include "Database/Terms.hpp"
#include "Database/Vehicle.hpp"

#include <nlohmann/json.hpp>

class System {
public:
  NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(System, airship, armorTypes, attackMotions, battleBgm, attackMotions,
                                              battleback1Name, battleback2Name, battlerHue, battlerName, boat,
                                              currencyUnit, defeatMe, editMapId, elements, equipTypes, gameTitle,
                                              gameoverMe, locale, magicSkills, menuCommands, optDisplayTp, optDrawTitle,
                                              optExtraExp, optFloorDeath, optFollowers, optSideView, optSlipDeath,
                                              optTransparent, partyMembers, ship, skillTypes, sounds, startMapId,
                                              startX, startY, switches, terms, testBattlers, testTroopId, title1Name,
                                              title2Name, titleBgm, variables, tileSize, versionId, victoryMe,
                                              weaponTypes, windowTone);
  struct Motion {
    NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Motion, type, weaponImageId);
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
};