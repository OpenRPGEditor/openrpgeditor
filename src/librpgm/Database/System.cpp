#include "Database/System.hpp"

#include <fstream>

System::Motion::Motion(const Motion& other)
: IModifiable(other)
, m_type(other.m_type)
, m_weaponImageId(other.m_weaponImageId)
, m_oldtype(other.m_oldtype)
, m_oldweaponImageId(other.m_oldweaponImageId) {}
System::Motion& System::Motion::operator=(const Motion& other) {
  IModifiable::operator=(other);
  m_type = other.m_type;
  m_weaponImageId = other.m_weaponImageId;
  m_oldtype = other.m_oldtype;
  m_oldweaponImageId = other.m_oldweaponImageId;
  return *this;
}
System::Motion::Motion(Motion&& other) noexcept
: IModifiable(other)
, m_type(other.m_type)
, m_weaponImageId(other.m_weaponImageId)
, m_oldtype(other.m_oldtype)
, m_oldweaponImageId(other.m_oldweaponImageId) {}
System::Motion& System::Motion::operator=(Motion&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_type = other.m_type;
  m_weaponImageId = other.m_weaponImageId;
  m_oldtype = other.m_oldtype;
  m_oldweaponImageId = other.m_oldweaponImageId;
  return *this;
}

MotionType System::Motion::type() const { return m_type; }
void System::Motion::setType(MotionType value) {
  if (m_type == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(type);
  m_type = value;
  if (!signalsDisabled()) {
    emit_signal(typeModified(), this, value);
  }
  setModified();
}

MotionImage System::Motion::weaponImageId() const { return m_weaponImageId; }
void System::Motion::setWeaponImageId(MotionImage value) {
  if (m_weaponImageId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(weaponImageId);
  m_weaponImageId = value;
  if (!signalsDisabled()) {
    emit_signal(weaponImageIdModified(), this, value);
  }
  setModified();
}

void System::Motion::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(type);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(weaponImageId);
}
void System::Motion::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(type);
  MODIFIABLE_ACCEPT_VALUE(weaponImageId);
}
nlohmann::ordered_json System::Motion::serializeOldValues() const {
  return {
      MODIFIABLE_SERIALIZE_VALUE(type),
      MODIFIABLE_SERIALIZE_VALUE(weaponImageId),
  };
}
rpgmutils::signal<void(System::Motion*, MotionType)>& System::Motion::typeModified() {
  if (!m_typeModified) {
    m_typeModified.emplace();
  }
  return *m_typeModified;
}
rpgmutils::signal<void(System::Motion*, MotionImage)>& System::Motion::weaponImageIdModified() {
  if (!m_weaponImageIdModified) {
    m_weaponImageIdModified.emplace();
  }
  return *m_weaponImageIdModified;
}

System::System(const System& other)
: IModifiable(other)
, m_airship(other.m_airship)
, m_armorTypes(other.m_armorTypes)
, m_attackMotions(other.m_attackMotions)
, m_battleBgm(other.m_battleBgm)
, m_battleback1Name(other.m_battleback1Name)
, m_battleback2Name(other.m_battleback2Name)
, m_battlerHue(other.m_battlerHue)
, m_battlerName(other.m_battlerName)
, m_boat(other.m_boat)
, m_currencyUnit(other.m_currencyUnit)
, m_defeatMe(other.m_defeatMe)
, m_editMapId(other.m_editMapId)
, m_elements(other.m_elements)
, m_equipTypes(other.m_equipTypes)
, m_gameTitle(other.m_gameTitle)
, m_gameoverMe(other.m_gameoverMe)
, m_locale(other.m_locale)
, m_magicSkills(other.m_magicSkills)
, m_menuCommands(other.m_menuCommands)
, m_optDisplayTp(other.m_optDisplayTp)
, m_optDrawTitle(other.m_optDrawTitle)
, m_optExtraExp(other.m_optExtraExp)
, m_optFloorDeath(other.m_optFloorDeath)
, m_optFollowers(other.m_optFollowers)
, m_optSlipDeath(other.m_optSlipDeath)
, m_optTransparent(other.m_optTransparent)
, m_partyMembers(other.m_partyMembers)
, m_ship(other.m_ship)
, m_skillTypes(other.m_skillTypes)
, m_sounds(other.m_sounds)
, m_startMapId(other.m_startMapId)
, m_startX(other.m_startX)
, m_startY(other.m_startY)
, m_switches(other.m_switches)
, m_terms(other.m_terms)
, m_testBattlers(other.m_testBattlers)
, m_testTroopId(other.m_testTroopId)
, m_title1Name(other.m_title1Name)
, m_title2Name(other.m_title2Name)
, m_titleBgm(other.m_titleBgm)
, m_variables(other.m_variables)
, m_versionId(other.m_versionId)
, m_victoryMe(other.m_victoryMe)
, m_weaponTypes(other.m_weaponTypes)
, m_windowTone(other.m_windowTone)
, m_hasEncryptedImages(other.m_hasEncryptedImages)
, m_hasEncryptedAudio(other.m_hasEncryptedAudio)
, m_encryptionKey(other.m_encryptionKey)
, m_oldairship(other.m_oldairship)
, m_oldarmorTypes(other.m_oldarmorTypes)
, m_oldattackMotions(other.m_oldattackMotions)
, m_oldbattleBgm(other.m_oldbattleBgm)
, m_oldbattleback1Name(other.m_oldbattleback1Name)
, m_oldbattleback2Name(other.m_oldbattleback2Name)
, m_oldbattlerHue(other.m_oldbattlerHue)
, m_oldbattlerName(other.m_oldbattlerName)
, m_oldboat(other.m_oldboat)
, m_oldcurrencyUnit(other.m_oldcurrencyUnit)
, m_olddefeatMe(other.m_olddefeatMe)
, m_oldeditMapId(other.m_oldeditMapId)
, m_oldelements(other.m_oldelements)
, m_oldequipTypes(other.m_oldequipTypes)
, m_oldgameTitle(other.m_oldgameTitle)
, m_oldgameoverMe(other.m_oldgameoverMe)
, m_oldlocale(other.m_oldlocale)
, m_oldmagicSkills(other.m_oldmagicSkills)
, m_oldmenuCommands(other.m_oldmenuCommands)
, m_oldoptDisplayTp(other.m_oldoptDisplayTp)
, m_oldoptDrawTitle(other.m_oldoptDrawTitle)
, m_oldoptExtraExp(other.m_oldoptExtraExp)
, m_oldoptFloorDeath(other.m_oldoptFloorDeath)
, m_oldoptFollowers(other.m_oldoptFollowers)
, m_oldoptSlipDeath(other.m_oldoptSlipDeath)
, m_oldoptTransparent(other.m_oldoptTransparent)
, m_oldpartyMembers(other.m_oldpartyMembers)
, m_oldship(other.m_oldship)
, m_oldskillTypes(other.m_oldskillTypes)
, m_oldsounds(other.m_oldsounds)
, m_oldstartMapId(other.m_oldstartMapId)
, m_oldstartX(other.m_oldstartX)
, m_oldstartY(other.m_oldstartY)
, m_oldswitches(other.m_oldswitches)
, m_oldterms(other.m_oldterms)
, m_oldtestBattlers(other.m_oldtestBattlers)
, m_oldtestTroopId(other.m_oldtestTroopId)
, m_oldtitle1Name(other.m_oldtitle1Name)
, m_oldtitle2Name(other.m_oldtitle2Name)
, m_oldtitleBgm(other.m_oldtitleBgm)
, m_oldvariables(other.m_oldvariables)
, m_oldversionId(other.m_oldversionId)
, m_oldvictoryMe(other.m_oldvictoryMe)
, m_oldweaponTypes(other.m_oldweaponTypes)
, m_oldwindowTone(other.m_oldwindowTone)
, m_oldhasEncryptedImages(other.m_oldhasEncryptedImages)
, m_oldhasEncryptedAudio(other.m_oldhasEncryptedAudio)
, m_oldencryptionKey(other.m_oldencryptionKey) {}
System& System::operator=(const System& other) {
  IModifiable::operator=(other);
  m_airship = other.m_airship;
  m_armorTypes = other.m_armorTypes;
  m_attackMotions = other.m_attackMotions;
  m_battleBgm = other.m_battleBgm;
  m_battleback1Name = other.m_battleback1Name;
  m_battleback2Name = other.m_battleback2Name;
  m_battlerHue = other.m_battlerHue;
  m_battlerName = other.m_battlerName;
  m_boat = other.m_boat;
  m_currencyUnit = other.m_currencyUnit;
  m_defeatMe = other.m_defeatMe;
  m_editMapId = other.m_editMapId;
  m_elements = other.m_elements;
  m_equipTypes = other.m_equipTypes;
  m_gameTitle = other.m_gameTitle;
  m_gameoverMe = other.m_gameoverMe;
  m_locale = other.m_locale;
  m_magicSkills = other.m_magicSkills;
  m_menuCommands = other.m_menuCommands;
  m_optDisplayTp = other.m_optDisplayTp;
  m_optDrawTitle = other.m_optDrawTitle;
  m_optExtraExp = other.m_optExtraExp;
  m_optFloorDeath = other.m_optFloorDeath;
  m_optFollowers = other.m_optFollowers;
  m_optSlipDeath = other.m_optSlipDeath;
  m_optTransparent = other.m_optTransparent;
  m_partyMembers = other.m_partyMembers;
  m_ship = other.m_ship;
  m_skillTypes = other.m_skillTypes;
  m_sounds = other.m_sounds;
  m_startMapId = other.m_startMapId;
  m_startX = other.m_startX;
  m_startY = other.m_startY;
  m_switches = other.m_switches;
  m_terms = other.m_terms;
  m_testBattlers = other.m_testBattlers;
  m_testTroopId = other.m_testTroopId;
  m_title1Name = other.m_title1Name;
  m_title2Name = other.m_title2Name;
  m_titleBgm = other.m_titleBgm;
  m_variables = other.m_variables;
  m_versionId = other.m_versionId;
  m_victoryMe = other.m_victoryMe;
  m_weaponTypes = other.m_weaponTypes;
  m_windowTone = other.m_windowTone;
  m_hasEncryptedImages = other.m_hasEncryptedImages;
  m_hasEncryptedAudio = other.m_hasEncryptedAudio;
  m_encryptionKey = other.m_encryptionKey;
  m_oldairship = other.m_oldairship;
  m_oldarmorTypes = other.m_oldarmorTypes;
  m_oldattackMotions = other.m_oldattackMotions;
  m_oldbattleBgm = other.m_oldbattleBgm;
  m_oldbattleback1Name = other.m_oldbattleback1Name;
  m_oldbattleback2Name = other.m_oldbattleback2Name;
  m_oldbattlerHue = other.m_oldbattlerHue;
  m_oldbattlerName = other.m_oldbattlerName;
  m_oldboat = other.m_oldboat;
  m_oldcurrencyUnit = other.m_oldcurrencyUnit;
  m_olddefeatMe = other.m_olddefeatMe;
  m_oldeditMapId = other.m_oldeditMapId;
  m_oldelements = other.m_oldelements;
  m_oldequipTypes = other.m_oldequipTypes;
  m_oldgameTitle = other.m_oldgameTitle;
  m_oldgameoverMe = other.m_oldgameoverMe;
  m_oldlocale = other.m_oldlocale;
  m_oldmagicSkills = other.m_oldmagicSkills;
  m_oldmenuCommands = other.m_oldmenuCommands;
  m_oldoptDisplayTp = other.m_oldoptDisplayTp;
  m_oldoptDrawTitle = other.m_oldoptDrawTitle;
  m_oldoptExtraExp = other.m_oldoptExtraExp;
  m_oldoptFloorDeath = other.m_oldoptFloorDeath;
  m_oldoptFollowers = other.m_oldoptFollowers;
  m_oldoptSlipDeath = other.m_oldoptSlipDeath;
  m_oldoptTransparent = other.m_oldoptTransparent;
  m_oldpartyMembers = other.m_oldpartyMembers;
  m_oldship = other.m_oldship;
  m_oldskillTypes = other.m_oldskillTypes;
  m_oldsounds = other.m_oldsounds;
  m_oldstartMapId = other.m_oldstartMapId;
  m_oldstartX = other.m_oldstartX;
  m_oldstartY = other.m_oldstartY;
  m_oldswitches = other.m_oldswitches;
  m_oldterms = other.m_oldterms;
  m_oldtestBattlers = other.m_oldtestBattlers;
  m_oldtestTroopId = other.m_oldtestTroopId;
  m_oldtitle1Name = other.m_oldtitle1Name;
  m_oldtitle2Name = other.m_oldtitle2Name;
  m_oldtitleBgm = other.m_oldtitleBgm;
  m_oldvariables = other.m_oldvariables;
  m_oldversionId = other.m_oldversionId;
  m_oldvictoryMe = other.m_oldvictoryMe;
  m_oldweaponTypes = other.m_oldweaponTypes;
  m_oldwindowTone = other.m_oldwindowTone;
  m_oldhasEncryptedImages = other.m_oldhasEncryptedImages;
  m_oldhasEncryptedAudio = other.m_oldhasEncryptedAudio;
  m_oldencryptionKey = other.m_oldencryptionKey;
  return *this;
}
System::System(System&& other) noexcept
: IModifiable(std::move(other))
, m_airship(std::move(other.m_airship))
, m_armorTypes(std::move(other.m_armorTypes))
, m_attackMotions(std::move(other.m_attackMotions))
, m_battleBgm(std::move(other.m_battleBgm))
, m_battleback1Name(std::move(other.m_battleback1Name))
, m_battleback2Name(std::move(other.m_battleback2Name))
, m_battlerHue(other.m_battlerHue)
, m_battlerName(std::move(other.m_battlerName))
, m_boat(std::move(other.m_boat))
, m_currencyUnit(std::move(other.m_currencyUnit))
, m_defeatMe(std::move(other.m_defeatMe))
, m_editMapId(other.m_editMapId)
, m_elements(std::move(other.m_elements))
, m_equipTypes(std::move(other.m_equipTypes))
, m_gameoverMe(std::move(other.m_gameoverMe))
, m_locale(std::move(other.m_locale))
, m_magicSkills(std::move(other.m_magicSkills))
, m_menuCommands(std::move(other.m_menuCommands))
, m_optDisplayTp(other.m_optDisplayTp)
, m_optDrawTitle(other.m_optDrawTitle)
, m_optExtraExp(other.m_optExtraExp)
, m_optFloorDeath(other.m_optFloorDeath)
, m_optFollowers(other.m_optFollowers)
, m_optSlipDeath(other.m_optSlipDeath)
, m_optTransparent(other.m_optTransparent)
, m_partyMembers(std::move(other.m_partyMembers))
, m_ship(std::move(other.m_ship))
, m_skillTypes(std::move(other.m_skillTypes))
, m_sounds(std::move(other.m_sounds))
, m_startMapId(other.m_startMapId)
, m_startX(other.m_startX)
, m_startY(other.m_startY)
, m_switches(std::move(other.m_switches))
, m_terms(std::move(other.m_terms))
, m_testBattlers(std::move(other.m_testBattlers))
, m_testTroopId(other.m_testTroopId)
, m_title1Name(std::move(other.m_title1Name))
, m_title2Name(std::move(other.m_title2Name))
, m_titleBgm(std::move(other.m_titleBgm))
, m_variables(std::move(other.m_variables))
, m_versionId(other.m_versionId)
, m_victoryMe(std::move(other.m_victoryMe))
, m_weaponTypes(std::move(other.m_weaponTypes))
, m_windowTone(std::move(other.m_windowTone))
, m_hasEncryptedImages(other.m_hasEncryptedImages)
, m_hasEncryptedAudio(other.m_hasEncryptedAudio)
, m_encryptionKey(other.m_encryptionKey)
, m_oldairship(std::move(other.m_oldairship))
, m_oldarmorTypes(std::move(other.m_oldarmorTypes))
, m_oldattackMotions(std::move(other.m_oldattackMotions))
, m_oldbattleBgm(std::move(other.m_oldbattleBgm))
, m_oldbattleback1Name(std::move(other.m_oldbattleback1Name))
, m_oldbattleback2Name(std::move(other.m_oldbattleback2Name))
, m_oldbattlerHue(other.m_oldbattlerHue)
, m_oldbattlerName(std::move(other.m_oldbattlerName))
, m_oldboat(std::move(other.m_oldboat))
, m_oldcurrencyUnit(std::move(other.m_oldcurrencyUnit))
, m_olddefeatMe(std::move(other.m_olddefeatMe))
, m_oldeditMapId(other.m_oldeditMapId)
, m_oldelements(std::move(other.m_oldelements))
, m_oldequipTypes(std::move(other.m_oldequipTypes))
, m_oldgameoverMe(std::move(other.m_oldgameoverMe))
, m_oldlocale(std::move(other.m_oldlocale))
, m_oldmagicSkills(std::move(other.m_oldmagicSkills))
, m_oldmenuCommands(std::move(other.m_oldmenuCommands))
, m_oldoptDisplayTp(other.m_oldoptDisplayTp)
, m_oldoptDrawTitle(other.m_oldoptDrawTitle)
, m_oldoptExtraExp(other.m_oldoptExtraExp)
, m_oldoptFloorDeath(other.m_oldoptFloorDeath)
, m_oldoptFollowers(other.m_oldoptFollowers)
, m_oldoptSlipDeath(other.m_oldoptSlipDeath)
, m_oldoptTransparent(other.m_oldoptTransparent)
, m_oldpartyMembers(std::move(other.m_oldpartyMembers))
, m_oldship(std::move(other.m_oldship))
, m_oldskillTypes(std::move(other.m_oldskillTypes))
, m_oldsounds(std::move(other.m_oldsounds))
, m_oldstartMapId(other.m_oldstartMapId)
, m_oldstartX(other.m_oldstartX)
, m_oldstartY(other.m_oldstartY)
, m_oldswitches(std::move(other.m_oldswitches))
, m_oldterms(std::move(other.m_oldterms))
, m_oldtestBattlers(std::move(other.m_oldtestBattlers))
, m_oldtestTroopId(other.m_oldtestTroopId)
, m_oldtitle1Name(std::move(other.m_oldtitle1Name))
, m_oldtitle2Name(std::move(other.m_oldtitle2Name))
, m_oldtitleBgm(std::move(other.m_oldtitleBgm))
, m_oldvariables(std::move(other.m_oldvariables))
, m_oldversionId(other.m_oldversionId)
, m_oldvictoryMe(std::move(other.m_oldvictoryMe))
, m_oldweaponTypes(std::move(other.m_oldweaponTypes))
, m_oldwindowTone(std::move(other.m_oldwindowTone))
, m_oldhasEncryptedImages(other.m_oldhasEncryptedImages)
, m_oldhasEncryptedAudio(other.m_oldhasEncryptedAudio)
, m_oldencryptionKey(other.m_oldencryptionKey) {}
System& System::operator=(System&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_airship = std::move(other.m_airship);
  m_armorTypes = std::move(other.m_armorTypes);
  m_attackMotions = std::move(other.m_attackMotions);
  m_battleBgm = std::move(other.m_battleBgm);
  m_battleback1Name = std::move(other.m_battleback1Name);
  m_battleback2Name = std::move(other.m_battleback2Name);
  m_battlerHue = other.m_battlerHue;
  m_battlerName = std::move(other.m_battlerName);
  m_boat = std::move(other.m_boat);
  m_currencyUnit = std::move(other.m_currencyUnit);
  m_defeatMe = std::move(other.m_defeatMe);
  m_editMapId = other.m_editMapId;
  m_elements = std::move(other.m_elements);
  m_equipTypes = std::move(other.m_equipTypes);
  m_gameTitle = std::move(other.m_gameTitle);
  m_gameoverMe = std::move(other.m_gameoverMe);
  m_locale = std::move(other.m_locale);
  m_magicSkills = std::move(other.m_magicSkills);
  m_menuCommands = std::move(other.m_menuCommands);
  m_optDisplayTp = other.m_optDisplayTp;
  m_optDrawTitle = other.m_optDrawTitle;
  m_optExtraExp = other.m_optExtraExp;
  m_optFloorDeath = other.m_optFloorDeath;
  m_optFollowers = other.m_optFollowers;
  m_optSlipDeath = other.m_optSlipDeath;
  m_optTransparent = other.m_optTransparent;
  m_partyMembers = std::move(other.m_partyMembers);
  m_ship = std::move(other.m_ship);
  m_skillTypes = std::move(other.m_skillTypes);
  m_sounds = std::move(other.m_sounds);
  m_startMapId = other.m_startMapId;
  m_startX = other.m_startX;
  m_startY = other.m_startY;
  m_switches = std::move(other.m_switches);
  m_terms = std::move(other.m_terms);
  m_testBattlers = std::move(other.m_testBattlers);
  m_testTroopId = other.m_testTroopId;
  m_title1Name = std::move(other.m_title1Name);
  m_title2Name = std::move(other.m_title2Name);
  m_titleBgm = std::move(other.m_titleBgm);
  m_variables = std::move(other.m_variables);
  m_versionId = other.m_versionId;
  m_victoryMe = std::move(other.m_victoryMe);
  m_weaponTypes = std::move(other.m_weaponTypes);
  m_windowTone = std::move(other.m_windowTone);
  m_hasEncryptedImages = other.m_hasEncryptedImages;
  m_hasEncryptedAudio = other.m_hasEncryptedAudio;
  m_encryptionKey = other.m_encryptionKey;
  m_oldairship = std::move(other.m_oldairship);
  m_oldarmorTypes = std::move(other.m_oldarmorTypes);
  m_oldattackMotions = std::move(other.m_oldattackMotions);
  m_oldbattleBgm = std::move(other.m_oldbattleBgm);
  m_oldbattleback1Name = std::move(other.m_oldbattleback1Name);
  m_oldbattleback2Name = std::move(other.m_oldbattleback2Name);
  m_oldbattlerHue = other.m_oldbattlerHue;
  m_oldbattlerName = std::move(other.m_oldbattlerName);
  m_oldboat = std::move(other.m_oldboat);
  m_oldcurrencyUnit = std::move(other.m_oldcurrencyUnit);
  m_olddefeatMe = std::move(other.m_olddefeatMe);
  m_oldeditMapId = other.m_oldeditMapId;
  m_oldelements = std::move(other.m_oldelements);
  m_oldequipTypes = std::move(other.m_oldequipTypes);
  m_oldgameTitle = std::move(other.m_oldgameTitle);
  m_oldgameoverMe = std::move(other.m_oldgameoverMe);
  m_oldlocale = std::move(other.m_oldlocale);
  m_oldmagicSkills = std::move(other.m_oldmagicSkills);
  m_oldmenuCommands = std::move(other.m_oldmenuCommands);
  m_oldoptDisplayTp = other.m_oldoptDisplayTp;
  m_oldoptDrawTitle = other.m_oldoptDrawTitle;
  m_oldoptExtraExp = other.m_oldoptExtraExp;
  m_oldoptFloorDeath = other.m_oldoptFloorDeath;
  m_oldoptFollowers = other.m_oldoptFollowers;
  m_oldoptSlipDeath = other.m_oldoptSlipDeath;
  m_oldoptTransparent = other.m_oldoptTransparent;
  m_oldpartyMembers = std::move(other.m_oldpartyMembers);
  m_oldship = std::move(other.m_oldship);
  m_oldskillTypes = std::move(other.m_oldskillTypes);
  m_oldsounds = std::move(other.m_oldsounds);
  m_oldstartMapId = other.m_oldstartMapId;
  m_oldstartX = other.m_oldstartX;
  m_oldstartY = other.m_oldstartY;
  m_oldswitches = std::move(other.m_oldswitches);
  m_oldterms = std::move(other.m_oldterms);
  m_oldtestBattlers = std::move(other.m_oldtestBattlers);
  m_oldtestTroopId = other.m_oldtestTroopId;
  m_oldtitle1Name = std::move(other.m_oldtitle1Name);
  m_oldtitle2Name = std::move(other.m_oldtitle2Name);
  m_oldtitleBgm = std::move(other.m_oldtitleBgm);
  m_oldvariables = std::move(other.m_oldvariables);
  m_oldversionId = other.m_oldversionId;
  m_oldvictoryMe = std::move(other.m_oldvictoryMe);
  m_oldweaponTypes = std::move(other.m_oldweaponTypes);
  m_oldwindowTone = std::move(other.m_oldwindowTone);
  m_oldhasEncryptedImages = other.m_oldhasEncryptedImages;
  m_oldhasEncryptedAudio = other.m_oldhasEncryptedAudio;
  m_oldencryptionKey = other.m_oldencryptionKey;
  return *this;
}

Vehicle& System::airship() { return m_airship; }
const Vehicle& System::airship() const { return m_airship; }
void System::setAirship(const Vehicle& airship) {
  if (airship == m_airship) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(airship);
  m_airship = airship;
  if (!signalsDisabled()) {
    emit_signal(airshipModified(), this, airship);
  }
  setModified();
}

const std::vector<std::string>& System::armorTypes() const { return m_armorTypes; }
[[nodiscard]] std::string System::armorType(const int id) const {
  if (id < 0 || id >= m_armorTypes.size()) {
    return {};
  }
  return m_armorTypes[id];
}
void System::setArmorType(int id, const std::string& value) {
  if (id < 0 || id >= m_armorTypes.size()) {
    return;
  }
  if (m_armorTypes[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(armorTypes);
  m_armorTypes[id] = value;
  if (!signalsDisabled()) {
    emit_signal(armorTypesModified(), this, m_armorTypes);
  }
  setModified();
}
void System::resizeArmorTypes(int newSize) {
  if (newSize < 0 || newSize == static_cast<int>(m_armorTypes.size())) {
    return;
  }

  MODIFIABLE_SET_OLD_VALUE(armorTypes);
  m_armorTypes.resize(newSize);

  if (!signalsDisabled()) {
    emit_signal(armorTypesModified(), this, m_armorTypes);
  }

  setModified();
}

const std::vector<System::Motion>& System::attackMotions() const { return m_attackMotions; }
void System::setAttackMotions(const std::vector<Motion>& value) {
  if (m_attackMotions == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(attackMotions);
  m_attackMotions = value;
  if (!signalsDisabled()) {
    emit_signal(attackMotionsModified(), this, value);
  }
  setModified();
}

const Audio& System::battleBgm() const { return m_battleBgm; }
void System::setBattleBgm(const Audio& battleBgm) {
  if (m_battleBgm == battleBgm) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(battleBgm);
  m_battleBgm = battleBgm;
  if (!signalsDisabled()) {
    emit_signal(battleBgmModified(), this, m_battleBgm);
  }
  setModified();
}

const std::string& System::battleback1Name() const { return m_battleback1Name; }
void System::setBattleback1Name(const std::string& value) {
  if (m_battleback1Name == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(battleback1Name);
  m_battleback1Name = value;
  if (!signalsDisabled()) {
    emit_signal(battleback1NameModified(), this, m_battleback1Name);
  }
  setModified();
}

const std::string& System::battleback2Name() const { return m_battleback2Name; }
void System::setBattleback2Name(const std::string& value) {
  if (m_battleback2Name == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(battleback2Name);
  m_battleback2Name = value;
  if (!signalsDisabled()) {
    emit_signal(battleback2NameModified(), this, m_battleback2Name);
  }
  setModified();
}

int System::battlerHue() const { return m_battlerHue; }
void System::setBattlerHue(int value) {
  if (m_battlerHue == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(battlerHue);
  m_battlerHue = value;
  if (!signalsDisabled()) {
    emit_signal(battlerHueModified(), this, m_battlerHue);
  }
  setModified();
}

const std::string& System::battlerName() const { return m_battlerName; }
void System::setBattlerName(const std::string& value) {
  if (m_battlerName == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(battlerName);
  m_battlerName = value;
  if (!signalsDisabled()) {
    emit_signal(battlerNameModified(), this, m_battlerName);
  }
  setModified();
}

Vehicle& System::boat() { return m_boat; }
const Vehicle& System::boat() const { return m_boat; }

const std::string& System::currencyUnit() const { return m_currencyUnit; }
void System::setCurrencyUnit(const std::string& value) {
  if (m_currencyUnit == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(currencyUnit);
  m_currencyUnit = value;
  if (!signalsDisabled()) {
    emit_signal(currencyUnitModified(), this, m_currencyUnit);
  }
  setModified();
}

Audio& System::defeatMe() { return m_defeatMe; }
const Audio& System::defeatMe() const { return m_defeatMe; }
void System::setDefeatMe(const Audio& value) {
  if (m_defeatMe == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(defeatMe);
  m_defeatMe = value;
  if (!signalsDisabled()) {
    emit_signal(defeatMeModified(), this, m_defeatMe);
  }
  setModified();
}

int System::editMapId() const { return m_editMapId; }
void System::setEditMapId(int value) {
  if (m_editMapId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(editMapId);
  m_editMapId = value;
  if (!signalsDisabled()) {
    emit_signal(editMapIdModified(), this, m_editMapId);
  }
  const auto oldChanges = m_hasChanges;
  setModified();
  /* Hack to prevent project from being marked as dirty, but still notify hooked functions if System is modified */
  // TODO: Better dirty handling for transient editor state
  m_hasChanges = oldChanges;
  disableSignals();
  setModified(false);
  enableSignals();
}

const std::vector<std::string>& System::elements() const { return m_elements; }
void System::setElements(const std::vector<std::string>& value) {
  if (m_elements == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(elements);
  m_elements = value;
  if (!signalsDisabled()) {
    emit_signal(elementsModified(), this, m_elements);
  }
  setModified();
}

[[nodiscard]] std::string System::element(const int id) const {
  if (id < 0 || id >= m_elements.size()) {
    return {};
  }
  return m_elements[id];
}

void System::setElement(int id, const std::string& value) {
  if (id < 0 || id >= m_elements.size()) {
    return;
  }
  if (m_elements[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(elements);
  m_elements[id] = value;
  if (!signalsDisabled()) {
    emit_signal(elementsModified(), this, m_elements);
  }
  setModified();
}
void System::resizeElements(int newSize) {
  if (newSize < 0 || newSize == static_cast<int>(m_elements.size())) {
    return;
  }

  MODIFIABLE_SET_OLD_VALUE(elements);
  m_elements.resize(newSize);

  if (!signalsDisabled()) {
    emit_signal(elementsModified(), this, m_elements);
  }

  setModified();
}
const std::vector<std::string>& System::equipTypes() const { return m_equipTypes; }
void System::setEquipTypes(const std::vector<std::string>& value) {
  if (m_equipTypes == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(equipTypes);
  m_equipTypes = value;
  if (!signalsDisabled()) {
    emit_signal(equipTypesModified(), this, m_equipTypes);
  }
  setModified();
}

[[nodiscard]] std::string System::equipType(const int id) const {
  if (id < 0 || id >= m_equipTypes.size()) {
    return {};
  }
  return m_equipTypes[id];
}

void System::setEquipType(int id, const std::string& value) {
  if (id < 0 || id >= m_equipTypes.size()) {
    return;
  }
  if (m_equipTypes[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(equipTypes);
  m_equipTypes[id] = value;
  if (!signalsDisabled()) {
    emit_signal(equipTypesModified(), this, m_equipTypes);
  }
  setModified();
}
void System::resizeEquipTypes(int newSize) {
  if (newSize < 0 || newSize == static_cast<int>(m_equipTypes.size())) {
    return;
  }

  MODIFIABLE_SET_OLD_VALUE(equipTypes);
  m_equipTypes.resize(newSize);

  if (!signalsDisabled()) {
    emit_signal(equipTypesModified(), this, m_equipTypes);
  }

  setModified();
}

const std::string& System::gameTitle() const { return m_gameTitle; }
void System::setGameTitle(const std::string& value) {
  if (m_gameTitle == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(gameTitle);
  m_gameTitle = value;
  if (!signalsDisabled()) {
    emit_signal(gameTitleModified(), this, m_gameTitle);
  }
  setModified();
}

Audio& System::gameoverMe() { return m_gameoverMe; }
const Audio& System::gameoverMe() const { return m_gameoverMe; }
void System::setGameoverMe(const Audio& value) {
  if (m_gameoverMe == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(gameoverMe);
  m_gameoverMe = value;
  if (!signalsDisabled()) {
    emit_signal(gameoverMeModified(), this, m_gameoverMe);
  }
  setModified();
}

const std::string& System::locale() const { return m_locale; }
void System::setLocale(const std::string& value) {
  if (m_locale == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(locale);
  m_locale = value;
  if (!signalsDisabled()) {
    emit_signal(localeModified(), this, m_locale);
  }
  setModified();
}

const std::vector<int>& System::magicSkills() const { return m_magicSkills; }
void System::setMagicSkills(const std::vector<int>& value) {
  if (m_magicSkills == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(magicSkills);
  m_magicSkills = value;
  if (!signalsDisabled()) {
    emit_signal(magicSkillsModified(), this, m_magicSkills);
  }
  setModified();
}

const std::array<bool, 6>& System::menuCommands() const { return m_menuCommands; }
void System::setMenuCommands(const std::array<bool, 6>& value) {
  if (m_menuCommands == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(menuCommands);
  m_menuCommands = value;
  if (!signalsDisabled()) {
    emit_signal(menuCommandsModified(), this, m_menuCommands);
  }
  setModified();
}
void System::setMenuCommand(int idx, bool value) {
  if (idx < 0 || idx >= m_menuCommands.size()) {
    return;
  }
  if (m_menuCommands[idx] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(menuCommands);
  m_menuCommands[idx] = value;
  if (!signalsDisabled()) {
    emit_signal(menuCommandsModified(), this, m_menuCommands);
  }
  setModified();
}

bool System::optDisplayTp() const { return m_optDisplayTp; }
void System::setOptDisplayTp(bool value) {
  if (m_optDisplayTp == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optDisplayTp);
  m_optDisplayTp = value;
  if (!signalsDisabled()) {
    emit_signal(optDisplayTpModified(), this, m_optDisplayTp);
  }
  setModified();
}

bool System::optDrawTitle() const { return m_optDrawTitle; }
void System::setOptDrawTitle(bool value) {
  if (m_optDrawTitle == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optDrawTitle);
  m_optDrawTitle = value;
  if (!signalsDisabled()) {
    emit_signal(optDrawTitleModified(), this, m_optDrawTitle);
  }
  setModified();
}

bool System::optExtraExp() const { return m_optExtraExp; }
void System::setOptExtraExp(bool value) {
  if (m_optExtraExp == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optExtraExp);
  m_optExtraExp = value;
  if (!signalsDisabled()) {
    emit_signal(optExtraExpModified(), this, m_optExtraExp);
  }
  setModified();
}

bool System::optFloorDeath() const { return m_optFloorDeath; }
void System::setOptFloorDeath(bool value) {
  if (m_optFloorDeath == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optFloorDeath);
  m_optFloorDeath = value;
  if (!signalsDisabled()) {
    emit_signal(optFloorDeathModified(), this, m_optFloorDeath);
  }
  setModified();
}

bool System::optFollowers() const { return m_optFollowers; }
void System::setOptFollowers(bool value) {
  if (m_optFollowers == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optFollowers);
  m_optFollowers = value;
  if (!signalsDisabled()) {
    emit_signal(optFollowersModified(), this, m_optFollowers);
  }
  setModified();
}

bool System::optSideView() const { return m_optSideView; }
void System::setOptSideView(bool value) {
  if (m_optSideView == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optSideView);
  m_optSideView = value;
  if (!signalsDisabled()) {
    emit_signal(optSideViewModified(), this, m_optSideView);
  }
  setModified();
}

bool System::optSlipDeath() const { return m_optSlipDeath; }
void System::setOptSlipDeath(bool value) {
  if (m_optSlipDeath == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optSlipDeath);
  m_optSlipDeath = value;
  if (!signalsDisabled()) {
    emit_signal(optSlipDeathModified(), this, m_optSlipDeath);
  }
  setModified();
}

bool System::optTransparent() const { return m_optTransparent; }
void System::setOptTransparent(bool value) {
  if (m_optTransparent == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(optTransparent);
  m_optTransparent = value;
  if (!signalsDisabled()) {
    emit_signal(optTransparentModified(), this, m_optTransparent);
  }
  setModified();
}

const std::vector<int>& System::partyMembers() const { return m_partyMembers; }
void System::setPartyMembers(const std::vector<int>& value) {
  if (m_partyMembers == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(partyMembers);
  m_partyMembers = value;
  if (!signalsDisabled()) {
    emit_signal(partyMembersModified(), this, m_partyMembers);
  }
  setModified();
}

void System::setPartyMember(int id, int value) {
  if (id < 0 || id >= m_partyMembers.size()) {
    return;
  }
  if (m_partyMembers[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(partyMembers);
  m_partyMembers[id] = value;
  if (!signalsDisabled()) {
    emit_signal(partyMembersModified(), this, m_partyMembers);
  }
  setModified();
}
void System::addPartyMember(int id) {
  MODIFIABLE_SET_OLD_VALUE(partyMembers);
  m_partyMembers.emplace_back(id);
  if (!signalsDisabled()) {
    emit_signal(partyMembersModified(), this, m_partyMembers);
  }
  setModified();
}

Vehicle& System::ship() { return m_ship; }
const Vehicle& System::ship() const { return m_ship; }
void System::setShip(const Vehicle& ship) {
  if (m_ship == ship) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(ship);
  m_ship = ship;
  if (!signalsDisabled()) {
    emit_signal(shipModified(), this, m_ship);
  }
  setModified();
}

const std::vector<std::string>& System::skillTypes() const { return m_skillTypes; }
void System::setSkillTypes(const std::vector<std::string>& value) {
  if (m_skillTypes == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(skillTypes);
  m_skillTypes = value;
  if (!signalsDisabled()) {
    emit_signal(skillTypesModified(), this, m_skillTypes);
  }
  setModified();
}

[[nodiscard]] std::string System::skillType(const int id) const {
  if (id < 0 || id >= m_skillTypes.size()) {
    return {};
  }
  return m_skillTypes[id];
}

void System::setSkillType(int id, const std::string& value) {
  if (id < 0 || id >= m_skillTypes.size()) {
    return;
  }
  if (m_skillTypes[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(skillTypes);
  m_skillTypes[id] = value;
  if (!signalsDisabled()) {
    emit_signal(skillTypesModified(), this, m_skillTypes);
  }
  setModified();
}

void System::resizeSkillTypes(int newSize) {
  if (newSize < 0 || newSize == static_cast<int>(m_skillTypes.size())) {
    return;
  }

  MODIFIABLE_SET_OLD_VALUE(skillTypes);
  m_skillTypes.resize(newSize);

  if (!signalsDisabled()) {
    emit_signal(skillTypesModified(), this, m_skillTypes);
  }

  setModified();
}

std::vector<Audio>& System::sounds() { return m_sounds; }
const std::vector<Audio>& System::sounds() const { return m_sounds; }
void System::setSounds(const std::vector<Audio>& value) {
  if (m_sounds == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(sounds);
  m_sounds = value;
  if (!signalsDisabled()) {
    emit_signal(soundsModified(), this, m_sounds);
  }
  setModified();
}

int System::startMapId() const { return m_startMapId; }
void System::setStartMapId(int value) {
  if (m_startMapId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(startMapId);
  m_startMapId = value;
  if (!signalsDisabled()) {
    emit_signal(startMapIdModified(), this, m_startMapId);
  }
  setModified();
}

int System::startX() const { return m_startX; }
void System::setStartX(int value) {
  if (m_startX == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(startX);
  m_startX = value;
  if (!signalsDisabled()) {
    emit_signal(startXModified(), this, m_startX);
  }
  setModified();
}

int System::startY() const { return m_startY; }
void System::setStartY(int value) {
  if (m_startY == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(startY);
  m_startY = value;
  if (!signalsDisabled()) {
    emit_signal(startYModified(), this, m_startY);
  }
  setModified();
}

const std::vector<std::string>& System::switches() const { return m_switches; }
void System::setSwitches(const std::vector<std::string>& value) {
  if (m_switches == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switches);
  m_switches = value;
  if (!signalsDisabled()) {
    emit_signal(switchesModified(), this, m_switches);
  }
  setModified();
}

std::string System::switche(const int id) const {
  if (id < 0 || id >= m_switches.size()) {
    return {};
  }
  return m_switches[id];
}

void System::setSwitch(int id, const std::string& value) {
  if (id < 0 || id >= m_switches.size()) {
    return;
  }
  if (m_switches[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(switches);
  m_switches[id] = value;
  if (!signalsDisabled()) {
    emit_signal(switchesModified(), this, m_switches);
  }
  setModified();
}

Terms& System::terms() { return m_terms; }
const Terms& System::terms() const { return m_terms; }
void System::setTerms(const Terms& value) {
  if (m_terms == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(terms);
  m_terms = value;
  if (!signalsDisabled()) {
    emit_signal(termsModified(), this, m_terms);
  }
  setModified();
}

const std::vector<Battler>& System::testBattlers() const { return m_testBattlers; }
void System::setTestBattlers(const std::vector<Battler>& value) {
  if (m_testBattlers == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(testBattlers);
  m_testBattlers = value;
  if (!signalsDisabled()) {
    emit_signal(testBattlersModified(), this, m_testBattlers);
  }
  setModified();
}

int System::testTroopId() const { return m_testTroopId; }
void System::setTroopTestId(const int value) {
  if (m_testTroopId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(testTroopId);
  m_testTroopId = value;
  if (!signalsDisabled()) {
    emit_signal(testTroopIdModified(), this, m_testTroopId);
  }
  setModified();
}

const std::string& System::title1Name() const { return m_title1Name; }
void System::setTitle1Name(const std::string& value) {
  if (m_title1Name == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(title1Name);
  m_title1Name = value;
  if (!signalsDisabled()) {
    emit_signal(title1NameModified(), this, m_title1Name);
  }
  setModified();
}

const std::string& System::title2Name() const { return m_title2Name; }
void System::setTitle2Name(const std::string& value) {
  if (m_title2Name == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(title2Name);
  m_title2Name = value;
  if (!signalsDisabled()) {
    emit_signal(title2NameModified(), this, m_title2Name);
  }
  setModified();
}

Audio& System::titleBgm() { return m_titleBgm; }
const Audio& System::titleBgm() const { return m_titleBgm; }
void System::setTitleBgm(const Audio& value) {
  if (m_titleBgm == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(titleBgm);
  m_titleBgm = value;
  if (!signalsDisabled()) {
    emit_signal(titleBgmModified(), this, m_titleBgm);
  }
  setModified();
}

const std::vector<std::string>& System::variables() const { return m_variables; }
[[nodiscard]] std::string System::variable(const int id) const {
  if (id < 0 || id >= m_variables.size()) {
    return {};
  }
  return m_variables[id];
}

void System::setVariable(int id, const std::string& value) {
  if (id < 0 || id >= m_variables.size()) {
    return;
  }
  if (m_variables[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(variables);
  m_variables[id] = value;
  if (!signalsDisabled()) {
    emit_signal(variablesModified(), this, m_variables);
  }
  setModified();
}

int System::tileSize() const { return m_tileSize; }
void System::setTileSize(const int value) {
  if (m_tileSize == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(tileSize);
  m_tileSize = value;
  if (!signalsDisabled()) {
    emit_signal(tileSizeModified(), this, m_tileSize);
  }
  setModified();
}

uint32_t System::versionId() { return m_versionId; }
void System::setVersionId(const uint32_t value) {
  if (m_versionId == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(versionId);
  m_versionId = value;
  if (!signalsDisabled()) {
    emit_signal(versionIdModified(), this, m_versionId);
  }
  setModified();
}

Audio& System::victoryMe() { return m_victoryMe; }
const Audio& System::victoryMe() const { return m_victoryMe; }
void System::setVictoryMe(const Audio& value) {
  if (m_victoryMe == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(victoryMe);
  m_victoryMe = value;
  if (!signalsDisabled()) {
    emit_signal(victoryMeModified(), this, m_victoryMe);
  }
  setModified();
}

const std::vector<std::string>& System::weaponTypes() const { return m_weaponTypes; }
void System::setWeaponTypes(const std::vector<std::string>& value) {
  if (m_weaponTypes == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(weaponTypes);
  m_weaponTypes = value;
  if (!signalsDisabled()) {
    emit_signal(weaponTypesModified(), this, value);
  }
  setModified();
}

[[nodiscard]] std::string System::weaponType(const int id) const {
  if (id < 0 || id >= m_weaponTypes.size()) {
    return {};
  }
  return m_weaponTypes[id];
}

void System::setWeaponType(int id, std::string_view value) {
  if (id < 0 || id >= m_weaponTypes.size()) {
    return;
  }
  if (m_weaponTypes[id] == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(weaponTypes);
  m_weaponTypes[id] = value;
  if (!signalsDisabled()) {
    emit_signal(weaponTypesModified(), this, m_weaponTypes);
  }
  setModified();
}
void System::resizeWeaponTypes(int newSize) {
  if (newSize < 0 || newSize == static_cast<int>(m_weaponTypes.size())) {
    return;
  }

  MODIFIABLE_SET_OLD_VALUE(weaponTypes);
  m_weaponTypes.resize(newSize);

  if (!signalsDisabled()) {
    emit_signal(weaponTypesModified(), this, m_weaponTypes);
  }

  setModified();
}

const std::array<int, 3>& System::windowTone() const { return m_windowTone; }
void System::setWindowTone(const std::array<int, 3>& value) {
  if (m_windowTone == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(windowTone);
  m_windowTone = value;
  if (!signalsDisabled()) {
    emit_signal(windowToneModified(), this, m_windowTone);
  }
  setModified();
}

bool System::hasEncryptedImages() const { return m_hasEncryptedImages ? *m_hasEncryptedImages : false; }
void System::setHasEncryptedImages(bool value) {
  if (m_hasEncryptedImages == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(hasEncryptedImages);
  m_hasEncryptedImages = value;
  if (!signalsDisabled()) {
    emit_signal(hasEncryptedImagesModified(), this, value);
  }
  setModified();
}
bool System::hasEncryptedAudio() const { return m_hasEncryptedAudio ? *m_hasEncryptedAudio : false; }
void System::setHasEncryptedAudio(bool value) {
  if (m_hasEncryptedAudio == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(hasEncryptedAudio);
  m_hasEncryptedAudio = value;
  if (!signalsDisabled()) {
    emit_signal(hasEncryptedAudioModified(), this, value);
  }
  setModified();
}
std::string System::encryptionKey() const { return m_encryptionKey ? *m_encryptionKey : std::string(); }
void System::setEncryptionKey(const std::string& value) {
  if (m_encryptionKey == value) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(encryptionKey);
  m_encryptionKey = value;
  if (!signalsDisabled()) {
    emit_signal(encryptionKeyModified(), this, value);
  }
  setModified();
}

[[nodiscard]] std::vector<char> System::encryptionKeyAsBytes() const {
  if (!m_encryptionKey) {
    return {};
  }

  std::vector<char> bytes;

  for (unsigned int i = 0; i < m_encryptionKey->length(); i += 2) {
    std::string byteString = m_encryptionKey->substr(i, 2);
    char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
    bytes.push_back(byte);
  }

  return bytes;
}

void System::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(airship);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(armorTypes);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(attackMotions);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battleBgm);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battleback1Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battleback2Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battlerHue);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(battlerName);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(boat);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(currencyUnit);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(defeatMe);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(editMapId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(elements);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(equipTypes);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(gameTitle);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(gameoverMe);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(locale);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(magicSkills);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(menuCommands); // Item, Skill, Equip, Status, Formation, Save;
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optDisplayTp);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optDrawTitle);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optExtraExp);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optFloorDeath);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optFollowers);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optSideView);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optSlipDeath);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(optTransparent);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(partyMembers);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(ship);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(skillTypes);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(sounds);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(startMapId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(startX);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(startY);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(switches);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(terms);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(testBattlers);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(testTroopId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(title1Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(title2Name);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(titleBgm);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(variables);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(tileSize);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(versionId);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(victoryMe);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(weaponTypes);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(windowTone);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(hasEncryptedImages);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(hasEncryptedAudio);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(encryptionKey);
  m_airship.restoreOriginal();
  std::ranges::for_each(m_attackMotions, [&](auto motion) { motion.restoreOriginal(); });
  m_battleBgm.restoreOriginal();
  m_boat.restoreOriginal();
  m_defeatMe.restoreOriginal();
  m_gameoverMe.restoreOriginal();
  std::ranges::for_each(m_sounds, [&](auto sound) { sound.restoreOriginal(); });
  m_terms.restoreOriginal();
  std::ranges::for_each(m_testBattlers, [&](auto battler) { battler.restoreOriginal(); });
  m_titleBgm.restoreOriginal();
  m_victoryMe.restoreOriginal();
}

void System::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(airship);
  MODIFIABLE_ACCEPT_VALUE(armorTypes);
  MODIFIABLE_ACCEPT_VALUE(attackMotions);
  MODIFIABLE_ACCEPT_VALUE(battleBgm);
  MODIFIABLE_ACCEPT_VALUE(battleback1Name);
  MODIFIABLE_ACCEPT_VALUE(battleback2Name);
  MODIFIABLE_ACCEPT_VALUE(battlerHue);
  MODIFIABLE_ACCEPT_VALUE(battlerName);
  MODIFIABLE_ACCEPT_VALUE(boat);
  MODIFIABLE_ACCEPT_VALUE(currencyUnit);
  MODIFIABLE_ACCEPT_VALUE(defeatMe);
  MODIFIABLE_ACCEPT_VALUE(editMapId);
  MODIFIABLE_ACCEPT_VALUE(elements);
  MODIFIABLE_ACCEPT_VALUE(equipTypes);
  MODIFIABLE_ACCEPT_VALUE(gameTitle);
  MODIFIABLE_ACCEPT_VALUE(gameoverMe);
  MODIFIABLE_ACCEPT_VALUE(locale);
  MODIFIABLE_ACCEPT_VALUE(magicSkills);
  MODIFIABLE_ACCEPT_VALUE(menuCommands); // Item, Skill, Equip, Status, Formation, Save;
  MODIFIABLE_ACCEPT_VALUE(optDisplayTp);
  MODIFIABLE_ACCEPT_VALUE(optDrawTitle);
  MODIFIABLE_ACCEPT_VALUE(optExtraExp);
  MODIFIABLE_ACCEPT_VALUE(optFloorDeath);
  MODIFIABLE_ACCEPT_VALUE(optFollowers);
  MODIFIABLE_ACCEPT_VALUE(optSideView);
  MODIFIABLE_ACCEPT_VALUE(optSlipDeath);
  MODIFIABLE_ACCEPT_VALUE(optTransparent);
  MODIFIABLE_ACCEPT_VALUE(partyMembers);
  MODIFIABLE_ACCEPT_VALUE(ship);
  MODIFIABLE_ACCEPT_VALUE(skillTypes);
  MODIFIABLE_ACCEPT_VALUE(sounds);
  MODIFIABLE_ACCEPT_VALUE(startMapId);
  MODIFIABLE_ACCEPT_VALUE(startX);
  MODIFIABLE_ACCEPT_VALUE(startY);
  MODIFIABLE_ACCEPT_VALUE(switches);
  MODIFIABLE_ACCEPT_VALUE(terms);
  MODIFIABLE_ACCEPT_VALUE(testBattlers);
  MODIFIABLE_ACCEPT_VALUE(testTroopId);
  MODIFIABLE_ACCEPT_VALUE(title1Name);
  MODIFIABLE_ACCEPT_VALUE(title2Name);
  MODIFIABLE_ACCEPT_VALUE(titleBgm);
  MODIFIABLE_ACCEPT_VALUE(variables);
  MODIFIABLE_ACCEPT_VALUE(tileSize);
  MODIFIABLE_ACCEPT_VALUE(versionId);
  MODIFIABLE_ACCEPT_VALUE(victoryMe);
  MODIFIABLE_ACCEPT_VALUE(weaponTypes);
  MODIFIABLE_ACCEPT_VALUE(windowTone);
  MODIFIABLE_ACCEPT_VALUE(hasEncryptedImages);
  MODIFIABLE_ACCEPT_VALUE(hasEncryptedAudio);
  MODIFIABLE_ACCEPT_VALUE(encryptionKey);
  m_airship.acceptChanges();
  std::ranges::for_each(m_attackMotions, [&](auto motion) { motion.acceptChanges(); });
  m_battleBgm.acceptChanges();
  m_boat.acceptChanges();
  m_defeatMe.acceptChanges();
  m_gameoverMe.acceptChanges();
  std::ranges::for_each(m_sounds, [&](auto sound) { sound.acceptChanges(); });
  m_terms.acceptChanges();
  std::ranges::for_each(m_testBattlers, [&](auto battler) { battler.acceptChanges(); });
  m_titleBgm.acceptChanges();
  m_victoryMe.acceptChanges();
}

nlohmann::ordered_json System::serializeOldValues() const {
  nlohmann::ordered_json j = {
      MODIFIABLE_SERIALIZE_CHILD_VALUE(airship),
      MODIFIABLE_SERIALIZE_VALUE(armorTypes),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(attackMotions),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(battleBgm),
      MODIFIABLE_SERIALIZE_VALUE(battleback1Name),
      MODIFIABLE_SERIALIZE_VALUE(battleback2Name),
      MODIFIABLE_SERIALIZE_VALUE(battlerHue),
      MODIFIABLE_SERIALIZE_VALUE(battlerName),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(boat),
      MODIFIABLE_SERIALIZE_VALUE(currencyUnit),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(defeatMe),
      MODIFIABLE_SERIALIZE_VALUE(editMapId),
      MODIFIABLE_SERIALIZE_VALUE(elements),
      MODIFIABLE_SERIALIZE_VALUE(equipTypes),
      MODIFIABLE_SERIALIZE_VALUE(gameTitle),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(gameoverMe),
      MODIFIABLE_SERIALIZE_VALUE(locale),
      MODIFIABLE_SERIALIZE_VALUE(magicSkills),
      MODIFIABLE_SERIALIZE_VALUE(menuCommands),
      MODIFIABLE_SERIALIZE_VALUE(optDisplayTp),
      MODIFIABLE_SERIALIZE_VALUE(optDrawTitle),
      MODIFIABLE_SERIALIZE_VALUE(optExtraExp),
      MODIFIABLE_SERIALIZE_VALUE(optFloorDeath),
      MODIFIABLE_SERIALIZE_VALUE(optFollowers),
      MODIFIABLE_SERIALIZE_VALUE(optSideView),
      MODIFIABLE_SERIALIZE_VALUE(optSlipDeath),
      MODIFIABLE_SERIALIZE_VALUE(optTransparent),
      MODIFIABLE_SERIALIZE_VALUE(partyMembers),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(ship),
      MODIFIABLE_SERIALIZE_VALUE(skillTypes),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(sounds),
      MODIFIABLE_SERIALIZE_VALUE(startMapId),
      MODIFIABLE_SERIALIZE_VALUE(startX),
      MODIFIABLE_SERIALIZE_VALUE(startY),
      MODIFIABLE_SERIALIZE_VALUE(switches),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(terms),
      MODIFIABLE_SERIALIZE_VALUE_CHILD_ARRAY(testBattlers),
      MODIFIABLE_SERIALIZE_VALUE(testTroopId),
      MODIFIABLE_SERIALIZE_VALUE(title1Name),
      MODIFIABLE_SERIALIZE_VALUE(title2Name),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(titleBgm),
      MODIFIABLE_SERIALIZE_VALUE(variables),
      MODIFIABLE_SERIALIZE_VALUE(tileSize),
      MODIFIABLE_SERIALIZE_VALUE(versionId),
      MODIFIABLE_SERIALIZE_CHILD_VALUE(victoryMe),
      MODIFIABLE_SERIALIZE_VALUE(weaponTypes),
      MODIFIABLE_SERIALIZE_VALUE(windowTone),
  };
  if (m_hasEncryptedImages || m_oldhasEncryptedImages) {
    j["hasEncryptedImages"] = m_oldhasEncryptedImages ? *m_oldhasEncryptedImages : m_hasEncryptedImages ? *m_hasEncryptedImages : false;
  }
  if (m_hasEncryptedAudio || m_oldhasEncryptedAudio) {
    j["hasEncryptedAudio"] = m_hasEncryptedAudio ? *m_oldhasEncryptedAudio : m_hasEncryptedAudio ? *m_hasEncryptedImages : false;
  }
  if (m_encryptionKey || m_oldencryptionKey) {
    j["encryptionKey"] = m_oldencryptionKey ? *m_oldencryptionKey : m_encryptionKey ? *m_encryptionKey : std::string();
  }
  return j;
}

rpgmutils::signal<void(System*, const Vehicle&)>& System::airshipModified() {
  if (!m_airshipModified) {
    m_airshipModified.emplace();
  }
  return *m_airshipModified;
}
rpgmutils::signal<void(System*, const std::vector<std::string>&)>& System::armorTypesModified() {
  if (!m_armorTypesModified) {
    m_armorTypesModified.emplace();
  }
  return *m_armorTypesModified;
}
rpgmutils::signal<void(System*, const std::vector<System::Motion>&)>& System::attackMotionsModified() {
  if (!m_attackMotionsModified) {
    m_attackMotionsModified.emplace();
  }
  return *m_attackMotionsModified;
}
rpgmutils::signal<void(System*, const Audio&)>& System::battleBgmModified() {
  if (!m_battleBgmModified) {
    m_battleBgmModified.emplace();
  }
  return *m_battleBgmModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::battleback1NameModified() {
  if (!m_battleback1NameModified) {
    m_battleback1NameModified.emplace();
  }
  return *m_battleback1NameModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::battleback2NameModified() {
  if (!m_battleback2NameModified) {
    m_battleback2NameModified.emplace();
  }
  return *m_battleback2NameModified;
}
rpgmutils::signal<void(System*, int)>& System::battlerHueModified() {
  if (!m_battlerHueModified) {
    m_battlerHueModified.emplace();
  }
  return *m_battlerHueModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::battlerNameModified() {
  if (!m_battlerNameModified) {
    m_battlerNameModified.emplace();
  }
  return *m_battlerNameModified;
}
rpgmutils::signal<void(System*, const Vehicle&)>& System::boatModified() {
  if (!m_boatModified) {
    m_boatModified.emplace();
  }
  return *m_boatModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::currencyUnitModified() {
  if (!m_currencyUnitModified) {
    m_currencyUnitModified.emplace();
  }
  return *m_currencyUnitModified;
}
rpgmutils::signal<void(System*, const Audio&)>& System::defeatMeModified() {
  if (!m_defeatMeModified) {
    m_defeatMeModified.emplace();
  }
  return *m_defeatMeModified;
}
rpgmutils::signal<void(System*, int)>& System::editMapIdModified() {
  if (!m_editMapIdModified) {
    m_editMapIdModified.emplace();
  }
  return *m_editMapIdModified;
}
rpgmutils::signal<void(System*, const std::vector<std::string>&)>& System::elementsModified() {
  if (!m_elementsModified) {
    m_elementsModified.emplace();
  }
  return *m_elementsModified;
}
rpgmutils::signal<void(System*, const std::vector<std::string>&)>& System::equipTypesModified() {
  if (!m_equipTypesModified) {
    m_equipTypesModified.emplace();
  }
  return *m_equipTypesModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::gameTitleModified() {
  if (!m_gameTitleModified) {
    m_gameTitleModified.emplace();
  }
  return *m_gameTitleModified;
}
rpgmutils::signal<void(System*, const Audio&)>& System::gameoverMeModified() {
  if (!m_gameoverMeModified) {
    m_gameoverMeModified.emplace();
  }
  return *m_gameoverMeModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::localeModified() {
  if (!m_localeModified) {
    m_localeModified.emplace();
  }
  return *m_localeModified;
}
rpgmutils::signal<void(System*, const std::vector<int>&)>& System::magicSkillsModified() {
  if (!m_magicSkillsModified) {
    m_magicSkillsModified.emplace();
  }
  return *m_magicSkillsModified;
}
rpgmutils::signal<void(System*, const std::array<bool, 6>&)>& System::menuCommandsModified() {
  if (!m_menuCommandsModified) {
    m_menuCommandsModified.emplace();
  }
  return *m_menuCommandsModified;
}
rpgmutils::signal<void(System*, bool)>& System::optDisplayTpModified() {
  if (!m_optDisplayTpModified) {
    m_optDisplayTpModified.emplace();
  }
  return *m_optDisplayTpModified;
}
rpgmutils::signal<void(System*, bool)>& System::optDrawTitleModified() {
  if (!m_optDrawTitleModified) {
    m_optDrawTitleModified.emplace();
  }
  return *m_optDrawTitleModified;
}
rpgmutils::signal<void(System*, bool)>& System::optExtraExpModified() {
  if (!m_optExtraExpModified) {
    m_optExtraExpModified.emplace();
  }
  return *m_optExtraExpModified;
}
rpgmutils::signal<void(System*, bool)>& System::optFloorDeathModified() {
  if (!m_optFloorDeathModified) {
    m_optFloorDeathModified.emplace();
  }
  return *m_optFloorDeathModified;
}
rpgmutils::signal<void(System*, bool)>& System::optFollowersModified() {
  if (!m_optFollowersModified) {
    m_optFollowersModified.emplace();
  }
  return *m_optFollowersModified;
}
rpgmutils::signal<void(System*, bool)>& System::optSideViewModified() {
  if (!m_optSideViewModified) {
    m_optSideViewModified.emplace();
  }
  return *m_optSideViewModified;
}
rpgmutils::signal<void(System*, bool)>& System::optSlipDeathModified() {
  if (!m_optSlipDeathModified) {
    m_optSlipDeathModified.emplace();
  }
  return *m_optSlipDeathModified;
}
rpgmutils::signal<void(System*, bool)>& System::optTransparentModified() {
  if (!m_optTransparentModified) {
    m_optTransparentModified.emplace();
  }
  return *m_optTransparentModified;
}
rpgmutils::signal<void(System*, const std::vector<int>&)>& System::partyMembersModified() {
  if (!m_partyMembersModified) {
    m_partyMembersModified.emplace();
  }
  return *m_partyMembersModified;
}
rpgmutils::signal<void(System*, const Vehicle&)>& System::shipModified() {
  if (!m_shipModified) {
    m_shipModified.emplace();
  }
  return *m_shipModified;
}
rpgmutils::signal<void(System*, const std::vector<std::string>&)>& System::skillTypesModified() {
  if (!m_skillTypesModified) {
    m_skillTypesModified.emplace();
  }
  return *m_skillTypesModified;
}
rpgmutils::signal<void(System*, const std::vector<Audio>&)>& System::soundsModified() {
  if (!m_soundsModified) {
    m_soundsModified.emplace();
  }
  return *m_soundsModified;
}
rpgmutils::signal<void(System*, int)>& System::startMapIdModified() {
  if (!m_startMapIdModified) {
    m_startMapIdModified.emplace();
  }
  return *m_startMapIdModified;
}
rpgmutils::signal<void(System*, int)>& System::startXModified() {
  if (!m_startXModified) {
    m_startXModified.emplace();
  }
  return *m_startXModified;
}
rpgmutils::signal<void(System*, int)>& System::startYModified() {
  if (!m_startYModified) {
    m_startYModified.emplace();
  }
  return *m_startYModified;
}
rpgmutils::signal<void(System*, const std::vector<std::string>&)>& System::switchesModified() {
  if (!m_switchesModified) {
    m_switchesModified.emplace();
  }
  return *m_switchesModified;
}
rpgmutils::signal<void(System*, const Terms&)>& System::termsModified() {
  if (!m_termsModified) {
    m_termsModified.emplace();
  }
  return *m_termsModified;
}
rpgmutils::signal<void(System*, const std::vector<Battler>&)>& System::testBattlersModified() {
  if (!m_testBattlersModified) {
    m_testBattlersModified.emplace();
  }
  return *m_testBattlersModified;
}
rpgmutils::signal<void(System*, int)>& System::testTroopIdModified() {
  if (!m_testTroopIdModified) {
    m_testTroopIdModified.emplace();
  }
  return *m_testTroopIdModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::title1NameModified() {
  if (!m_title1NameModified) {
    m_title1NameModified.emplace();
  }
  return *m_title1NameModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::title2NameModified() {
  if (!m_title2NameModified) {
    m_title2NameModified.emplace();
  }
  return *m_title2NameModified;
}
rpgmutils::signal<void(System*, const Audio&)>& System::titleBgmModified() {
  if (!m_titleBgmModified) {
    m_titleBgmModified.emplace();
  }
  return *m_titleBgmModified;
}
rpgmutils::signal<void(System*, const std::vector<std::string>&)>& System::variablesModified() {
  if (!m_variablesModified) {
    m_variablesModified.emplace();
  }
  return *m_variablesModified;
}
rpgmutils::signal<void(System*, int)>& System::tileSizeModified() {
  if (!m_tileSizeModified) {
    m_tileSizeModified.emplace();
  }
  return *m_tileSizeModified;
}
rpgmutils::signal<void(System*, uint32_t)>& System::versionIdModified() {
  if (!m_versionIdModified) {
    m_versionIdModified.emplace();
  }
  return *m_versionIdModified;
}
rpgmutils::signal<void(System*, const Audio&)>& System::victoryMeModified() {
  if (!m_victoryMeModified) {
    m_victoryMeModified.emplace();
  }
  return *m_victoryMeModified;
}
rpgmutils::signal<void(System*, const std::vector<std::string>&)>& System::weaponTypesModified() {
  if (m_weaponTypesModified) {
    m_weaponTypesModified.emplace();
  }
  return *m_weaponTypesModified;
}
rpgmutils::signal<void(System*, const std::array<int, 3>&)>& System::windowToneModified() {
  if (m_windowToneModified) {
    m_windowToneModified.emplace();
  }
  return *m_windowToneModified;
}
rpgmutils::signal<void(System*, bool)>& System::hasEncryptedImagesModified() {
  if (!m_hasEncryptedImagesModified) {
    m_hasEncryptedImagesModified.emplace();
  }
  return *m_hasEncryptedImagesModified;
}
rpgmutils::signal<void(System*, bool)>& System::hasEncryptedAudioModified() {
  if (!m_hasEncryptedAudioModified) {
    m_hasEncryptedAudioModified.emplace();
  }
  return *m_hasEncryptedAudioModified;
}
rpgmutils::signal<void(System*, const std::string&)>& System::encryptionKeyModified() {
  if (!m_encryptionKeyModified) {
    m_encryptionKeyModified.emplace();
  }
  return *m_encryptionKeyModified;
}

void to_json(nlohmann::ordered_json& j, const System::Motion& motion) {
  j = {
      {"type", motion.m_type},
      {"weaponImageId", motion.m_weaponImageId},
  };
}

void from_json(const nlohmann::ordered_json& j, System::Motion& motion) {
  motion.m_type = j.value("type", motion.m_type);
  motion.m_weaponImageId = j.value("weaponImageId", motion.m_weaponImageId);
}

void to_json(nlohmann::ordered_json& j, const System& system) {
  j = {
      {"airship", system.m_airship},
      {"armorTypes", system.m_armorTypes},
      {"attackMotions", system.m_attackMotions},
      {"battleBgm", system.m_battleBgm},
      {"battleback1Name", system.m_battleback1Name},
      {"battleback2Name", system.m_battleback2Name},
      {"battlerHue", system.m_battlerHue},
      {"battlerName", system.m_battlerName},
      {"boat", system.m_boat},
      {"currencyUnit", system.m_currencyUnit},
      {"defeatMe", system.m_defeatMe},
      {"editMapId", system.m_editMapId},
      {"elements", system.m_elements},
      {"equipTypes", system.m_equipTypes},
      {"gameTitle", system.m_gameTitle},
      {"gameoverMe", system.m_gameoverMe},
      {"locale", system.m_locale},
      {"magicSkills", system.m_magicSkills},
      {"menuCommands", system.m_menuCommands},
      {"optDisplayTp", system.m_optDisplayTp},
      {"optDrawTitle", system.m_optDrawTitle},
      {"optExtraExp", system.m_optExtraExp},
      {"optFloorDeath", system.m_optFloorDeath},
      {"optFollowers", system.m_optFollowers},
      {"optSideView", system.m_optSideView},
      {"optSlipDeath", system.m_optSlipDeath},
      {"optTransparent", system.m_optTransparent},
      {"partyMembers", system.m_partyMembers},
      {"ship", system.m_ship},
      {"skillTypes", system.m_skillTypes},
      {"sounds", system.m_sounds},
      {"startMapId", system.m_startMapId},
      {"startX", system.m_startX},
      {"startY", system.m_startY},
      {"switches", system.m_switches},
      {"terms", system.m_terms},
      {"testBattlers", system.m_testBattlers},
      {"testTroopId", system.m_testTroopId},
      {"title1Name", system.m_title1Name},
      {"title2Name", system.m_title2Name},
      {"titleBgm", system.m_titleBgm},
      {"variables", system.m_variables},
      {"tileSize", system.m_tileSize},
      {"versionId", system.m_versionId},
      {"victoryMe", system.m_victoryMe},
      {"weaponTypes", system.m_weaponTypes},
      {"windowTone", system.m_windowTone},
  };
}

void from_json(const nlohmann::ordered_json& j, System& system) {
  system.m_airship = j.value("airship", system.m_airship);
  system.m_armorTypes = j.value("armorTypes", system.m_armorTypes);
  system.m_attackMotions = j.value("attackMotions", system.m_attackMotions);
  system.m_battleBgm = j.value("battleBgm", system.m_battleBgm);
  system.m_battleback1Name = j.value("battleback1Name", system.m_battleback1Name);
  system.m_battleback2Name = j.value("battleback2Name", system.m_battleback2Name);
  system.m_battlerHue = j.value("battlerHue", system.m_battlerHue);
  system.m_battlerName = j.value("battlerName", system.m_battlerName);
  system.m_boat = j.value("boat", system.m_boat);
  system.m_currencyUnit = j.value("currencyUnit", system.m_currencyUnit);
  system.m_defeatMe = j.value("defeatMe", system.m_defeatMe);
  system.m_editMapId = j.value("editMapId", system.m_editMapId);
  system.m_elements = j.value("elements", system.m_elements);
  system.m_equipTypes = j.value("equipTypes", system.m_equipTypes);
  system.m_gameTitle = j.value("gameTitle", system.m_gameTitle);
  system.m_gameoverMe = j.value("gameoverMe", system.m_gameoverMe);
  system.m_locale = j.value("locale", system.m_locale);
  system.m_magicSkills = j.value("magicSkills", system.m_magicSkills);
  system.m_menuCommands = j.value("menuCommands", system.m_menuCommands);
  system.m_optDisplayTp = j.value("optDisplayTp", system.m_optDisplayTp);
  system.m_optDrawTitle = j.value("optDrawTitle", system.m_optDrawTitle);
  system.m_optExtraExp = j.value("optExtraExp", system.m_optExtraExp);
  system.m_optFloorDeath = j.value("optFloorDeath", system.m_optFloorDeath);
  system.m_optFollowers = j.value("optFollowers", system.m_optFollowers);
  system.m_optSideView = j.value("optSideView", system.m_optSideView);
  system.m_optSlipDeath = j.value("optSlipDeath", system.m_optSlipDeath);
  system.m_optTransparent = j.value("optTransparent", system.m_optTransparent);
  system.m_partyMembers = j.value("partyMembers", system.m_partyMembers);
  system.m_ship = j.value("ship", system.m_ship);
  system.m_skillTypes = j.value("skillTypes", system.m_skillTypes);
  system.m_sounds = j.value("sounds", system.m_sounds);
  system.m_startMapId = j.value("startMapId", system.m_startMapId);
  system.m_startX = j.value("startX", system.m_startX);
  system.m_startY = j.value("startY", system.m_startY);
  system.m_switches = j.value("switches", system.m_switches);
  system.m_terms = j.value("terms", system.m_terms);
  system.m_testBattlers = j.value("testBattlers", system.m_testBattlers);
  system.m_testTroopId = j.value("testTroopId", system.m_testTroopId);
  system.m_title1Name = j.value("title1Name", system.m_title1Name);
  system.m_title2Name = j.value("title2Name", system.m_title2Name);
  system.m_titleBgm = j.value("titleBgm", system.m_titleBgm);
  system.m_variables = j.value("variables", system.m_variables);
  system.m_tileSize = j.value("tileSize", system.m_tileSize);
  system.m_versionId = j.value("versionId", system.m_versionId);
  system.m_victoryMe = j.value("victoryMe", system.m_victoryMe);
  system.m_weaponTypes = j.value("weaponTypes", system.m_weaponTypes);
  system.m_windowTone = j.value("windowTone", system.m_windowTone);
}