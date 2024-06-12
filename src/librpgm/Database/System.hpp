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

  std::string& variable(int id) { return variables[id]; }
  const std::string& variable(int id) const { return variables[id]; }

  std::string& switche(int id) { return switches[id]; }
  const std::string& switche(int id) const { return switches[id]; }

  std::string& skillType(int id) { return skillTypes[id]; }
  const std::string& skillType(int id) const { return skillTypes[id]; }

  std::string& equipType(int id) { return equipTypes[id]; }
  const std::string& equipType(int id) const { return equipTypes[id]; }

private:
  std::vector<System> m_system;
};