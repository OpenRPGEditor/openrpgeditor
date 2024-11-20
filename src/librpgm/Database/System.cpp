#include "System.hpp"

#include <fstream>

void to_json(nlohmann::json& j, const System& system) {
  j = {
      {"airship", system.airship},
      {"armorTypes", system.armorTypes},
      {"attackMotions", system.attackMotions},
      {"battleback1Name", system.battleback1Name},
      {"battleback2Name", system.battleback2Name},
      {"battleBgm", system.battleBgm},
      {"battlerHue", system.battlerHue},
      {"battlerName", system.battlerName},
      {"boat", system.boat},
      {"currencyUnit", system.currencyUnit},
      {"defeatMe", system.defeatMe},
      {"editMapId", system.editMapId},
      {"elements", system.elements},
      {"equipTypes", system.equipTypes},
      {"gameTitle", system.gameTitle},
      {"gameoverMe", system.gameoverMe},
      {"locale", system.locale},
      {"magicSkills", system.magicSkills},
      {"menuCommands", system.menuCommands},
      {"optDisplayTp", system.optDisplayTp},
      {"optDrawTitle", system.optDrawTitle},
      {"optExtraExp", system.optExtraExp},
      {"optFloorDeath", system.optFloorDeath},
      {"optFollowers", system.optFollowers},
      {"optSideView", system.optSideView},
      {"optSlipDeath", system.optSlipDeath},
      {"optTransparent", system.optTransparent},
      {"partyMembers", system.partyMembers},
      {"ship", system.ship},
      {"skillTypes", system.skillTypes},
      {"sounds", system.sounds},
      {"startMapId", system.startMapId},
      {"startX", system.startX},
      {"startY", system.startY},
      {"switches", system.switches},
      {"terms", system.terms},
      {"testBattlers", system.testBattlers},
      {"testTroopId", system.testTroopId},
      {"title1Name", system.title1Name},
      {"title2Name", system.title2Name},
      {"variables", system.variables},
      {"tileSize", system.tileSize},
      {"versionId", system.versionId},
      {"victoryMe", system.victoryMe},
      {"weaponTypes", system.weaponTypes},
      {"windowTone", system.windowTone},
  };
}

void from_json(const nlohmann::json& j, System& system) {
  system.airship = j.value("airship", system.airship);
  system.armorTypes = j.value("armorTypes", system.armorTypes);
  system.attackMotions = j.value("attackMotions", system.attackMotions);
  system.battleBgm = j.value("battleBgm", system.battleBgm);
  system.battlerHue = j.value("battlerHue", system.battlerHue);
  system.battlerName = j.value("battlerName", system.battlerName);
  system.boat = j.value("boat", system.boat);
  system.currencyUnit = j.value("currencyUnit", system.currencyUnit);
  system.defeatMe = j.value("defeatMe", system.defeatMe);
  system.editMapId = j.value("editMapId", system.editMapId);
  system.elements = j.value("elements", system.elements);
  system.equipTypes = j.value("equipTypes", system.equipTypes);
  system.gameTitle = j.value("gameTitle", system.gameTitle);
  system.gameoverMe = j.value("gameoverMe", system.gameoverMe);
  system.locale = j.value("locale", system.locale);
  system.magicSkills = j.value("magicSkills", system.magicSkills);
  system.menuCommands = j.value("menuCommands", system.menuCommands);
  system.optDisplayTp = j.value("optDisplayTp", system.optDisplayTp);
  system.optDrawTitle = j.value("optDrawTitle", system.optDrawTitle);
  system.optExtraExp = j.value("optExtraExp", system.optExtraExp);
  system.optFloorDeath = j.value("optFloorDeath", system.optFloorDeath);
  system.optFollowers = j.value("optFollowers", system.optFollowers);
  system.optSideView = j.value("optSideView", system.optSideView);
  system.optSlipDeath = j.value("optSlipDeath", system.optSlipDeath);
  system.optTransparent = j.value("optTransparent", system.optTransparent);
  system.partyMembers = j.value("partyMembers", system.partyMembers);
  system.ship = j.value("ship", system.ship);
  system.startMapId = j.value("startMapId", system.startMapId);
  system.startX = j.value("startX", system.startX);
  system.startY = j.value("startY", system.startY);
  system.switches = j.value("switches", system.switches);
  system.terms = j.value("terms", system.terms);
  system.title1Name = j.value("title1Name", system.title1Name);
  system.title2Name = j.value("title2Name", system.title2Name);
  system.variables = j.value("variables", system.variables);
  system.tileSize = j.value("tileSize", system.tileSize);
  system.versionId = j.value("versionId", system.versionId);
  system.victoryMe = j.value("victoryMe", system.victoryMe);
  system.weaponTypes = j.value("weaponTypes", system.weaponTypes);
  system.windowTone = j.value("windowTone", system.windowTone);
}

System System::load(std::string_view filepath) {
  std::ifstream file(filepath.data());
  nlohmann::json data = nlohmann::json::parse(file);
  System ret = data.get<System>();
  data["hasEncryptedImages"].get_to(ret.hasEncryptedImages);
  data["hasEncryptedAudio"].get_to(ret.hasEncryptedAudio);
  data["encryptionKey"].get_to(ret.encryptionKey);
  return ret;
}

bool System::serialize(std::string_view filepath) {
  std::ofstream file(filepath.data());
  nlohmann::json data = *this;
  if (hasEncryptedImages || hasEncryptedAudio || encryptionKey) {
    data["hasEncryptedImages"] = *hasEncryptedImages;
    data["hasEncryptedAudio"] = *hasEncryptedAudio;
    data["encryptionKey"] = *encryptionKey;
  }
  file << data.dump(4);
  return true;
}
