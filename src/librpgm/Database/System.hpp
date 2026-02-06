#pragma once

#include "Database/Battler.hpp"
#include "Database/Globals.hpp"
#include "Database/Terms.hpp"
#include "Database/Vehicle.hpp"

class System final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& j, const System& system);
  friend void from_json(const nlohmann::ordered_json& j, System& system);
  friend class SystemSerializer;

public:
  class Motion final : public IModifiable {
    friend void to_json(nlohmann::ordered_json& j, const Motion& motion);
    friend void from_json(const nlohmann::ordered_json& j, Motion& motion);

  public:
    Motion() = default;
    Motion(const Motion& other);
    Motion& operator=(const Motion& other);
    Motion(Motion&& other) noexcept;
    Motion& operator=(Motion&& other) noexcept;

    MotionType type() const;
    void setType(MotionType value);

    MotionImage weaponImageId() const;
    void setWeaponImageId(MotionImage value);

    void restoreOriginal() override;
    void acceptChanges() override;
    nlohmann::ordered_json serializeOldValues() const override;

    rpgmutils::signal<void(Motion*, MotionType)>& typeModified();
    rpgmutils::signal<void(Motion*, MotionImage)>& weaponImageIdModified();

    bool operator==(const Motion& other) const { return m_type == other.m_type && m_weaponImageId == other.m_weaponImageId; }

  private:
    MotionType m_type = MotionType::Thrust;
    MotionImage m_weaponImageId = MotionImage::None;
    std::optional<MotionType> m_oldtype;
    std::optional<MotionImage> m_oldweaponImageId;
    std::optional<rpgmutils::signal<void(Motion*, MotionType)>> m_typeModified;
    std::optional<rpgmutils::signal<void(Motion*, MotionImage)>> m_weaponImageIdModified;
  };
  System() = default;
  System(const System& other);
  System& operator=(const System& other);
  System(System&& other) noexcept;
  System& operator=(System&& other) noexcept;

  Vehicle& airship();
  const Vehicle& airship() const;
  void setAirship(const Vehicle& airship);

  const std::vector<std::string>& armorTypes() const;
  [[nodiscard]] std::string armorType(int id) const;
  void setArmorType(int id, const std::string& value);
  void resizeArmorTypes(int size);

  const std::vector<Motion>& attackMotions() const;
  void setAttackMotions(const std::vector<Motion>& value);

  const Audio& battleBgm() const;
  void setBattleBgm(const Audio& battleBgm);

  const std::string& battleback1Name() const;
  void setBattleback1Name(const std::string& value);

  const std::string& battleback2Name() const;
  void setBattleback2Name(const std::string& value);

  int battlerHue() const;
  void setBattlerHue(int value);

  const std::string& battlerName() const;
  void setBattlerName(const std::string& value);

  Vehicle& boat();
  const Vehicle& boat() const;

  const std::string& currencyUnit() const;
  void setCurrencyUnit(const std::string& value);

  Audio& defeatMe();
  const Audio& defeatMe() const;
  void setDefeatMe(const Audio& value);

  int editMapId() const;
  void setEditMapId(int value);

  const std::vector<std::string>& elements() const;
  void setElements(const std::vector<std::string>& value);
  void resizeElements(int size);

  [[nodiscard]] std::string element(int id) const;
  void setElement(int id, const std::string& value);

  const std::vector<std::string>& equipTypes() const;
  void setEquipTypes(const std::vector<std::string>& value);

  [[nodiscard]] std::string equipType(int id) const;
  void setEquipType(int id, const std::string& value);
  void resizeEquipTypes(int size);

  const std::string& gameTitle() const;
  void setGameTitle(const std::string& value);

  Audio& gameoverMe();
  const Audio& gameoverMe() const;
  void setGameoverMe(const Audio& value);

  const std::string& locale() const;
  void setLocale(const std::string& value);

  const std::vector<int>& magicSkills() const;
  void setMagicSkills(const std::vector<int>& value);

  const std::array<bool, 6>& menuCommands() const;
  void setMenuCommands(const std::array<bool, 6>& value);
  void setMenuCommand(int idx, bool value);

  bool optDisplayTp() const;
  void setOptDisplayTp(bool value);

  bool optDrawTitle() const;
  void setOptDrawTitle(bool value);

  bool optExtraExp() const;
  void setOptExtraExp(bool value);

  bool optFloorDeath() const;
  void setOptFloorDeath(bool value);

  bool optFollowers() const;
  void setOptFollowers(bool value);

  bool optSideView() const;
  void setOptSideView(bool value);

  bool optSlipDeath() const;
  void setOptSlipDeath(bool value);

  bool optTransparent() const;
  void setOptTransparent(bool value);

  const std::vector<int>& partyMembers() const;
  void setPartyMembers(const std::vector<int>& value);

  void setPartyMember(int id, int value);
  void addPartyMember(int id);

  Vehicle& ship();
  const Vehicle& ship() const;
  void setShip(const Vehicle& ship);

  const std::vector<std::string>& skillTypes() const;
  void setSkillTypes(const std::vector<std::string>& value);

  [[nodiscard]] std::string skillType(int id) const;
  void setSkillType(int id, const std::string& value);
  void resizeSkillTypes(int size);

  std::vector<Audio>& sounds();
  const std::vector<Audio>& sounds() const;
  void setSounds(const std::vector<Audio>& value);

  int startMapId() const;
  void setStartMapId(int value);

  int startX() const;
  void setStartX(int value);

  int startY() const;
  void setStartY(int value);

  const std::vector<std::string>& switches() const;
  void setSwitches(const std::vector<std::string>& value);

  [[nodiscard]] std::string switche(int id) const;
  void setSwitch(int id, const std::string& value);
  void swapSwitchReference(int targetId, int newId) {
    if (targetId >= 0 && targetId < m_switches.size() && newId >= 0 && newId < m_switches.size()) {
      std::swap(m_switches[targetId], m_switches[newId]);
    }
  }

  Terms& terms();
  const Terms& terms() const;
  void setTerms(const Terms& value);

  const std::vector<Battler>& testBattlers() const;
  void setTestBattlers(const std::vector<Battler>& value);

  int testTroopId() const;
  void setTroopTestId(int value);

  const std::string& title1Name() const;
  void setTitle1Name(const std::string& value);

  const std::string& title2Name() const;
  void setTitle2Name(const std::string& value);

  Audio& titleBgm();
  const Audio& titleBgm() const;
  void setTitleBgm(const Audio& value);

  const std::vector<std::string>& variables() const;
  [[nodiscard]] std::string variable(int id) const;
  void setVariable(int id, const std::string& value);

  void swapVariableReference(int targetId, int newId) {
    if (targetId >= 0 && targetId < m_variables.size() && newId >= 0 && newId < m_variables.size()) {
      std::swap(m_variables[targetId], m_variables[newId]);
    }
  }
  int tileSize() const;
  void setTileSize(int value);

  uint32_t versionId();
  void setVersionId(uint32_t value);

  Audio& victoryMe();
  const Audio& victoryMe() const;
  void setVictoryMe(const Audio& value);

  const std::vector<std::string>& weaponTypes() const;
  void setWeaponTypes(const std::vector<std::string>& value);

  [[nodiscard]] std::string weaponType(int id) const;
  void setWeaponType(int id, std::string_view value);
  void resizeWeaponTypes(int size);

  const std::array<int, 3>& windowTone() const;
  void setWindowTone(const std::array<int, 3>& value);

  bool hasEncryptedImages() const;
  void setHasEncryptedImages(bool value);
  bool hasEncryptedAudio() const;
  void setHasEncryptedAudio(bool value);
  std::string encryptionKey() const;
  void setEncryptionKey(const std::string& value);

  [[nodiscard]] std::vector<char> encryptionKeyAsBytes() const;

  void restoreOriginal() override;
  void acceptChanges() override;

  bool isModified() const override {
    bool modified = IModifiable::isModified();
    modified |= m_airship.isModified();
    modified |= std::ranges::any_of(m_attackMotions, [&](auto motion) { return motion.isModified(); });
    modified |= m_battleBgm.isModified();
    modified |= m_boat.isModified();
    modified |= m_defeatMe.isModified();
    modified |= m_gameoverMe.isModified();
    modified |= std::ranges::any_of(m_sounds, [&](auto sound) { return sound.isModified(); });
    modified |= m_terms.isModified();
    modified |= std::ranges::any_of(m_testBattlers, [&](auto battler) { return battler.isModified(); });
    modified |= m_titleBgm.isModified();
    modified |= m_victoryMe.isModified();
    return modified;
  }
  nlohmann::ordered_json serializeOldValues() const override;
  rpgmutils::signal<void(System*, const Vehicle&)>& airshipModified();
  rpgmutils::signal<void(System*, const std::vector<std::string>&)>& armorTypesModified();
  rpgmutils::signal<void(System*, const std::vector<Motion>&)>& attackMotionsModified();
  rpgmutils::signal<void(System*, const Audio&)>& battleBgmModified();
  rpgmutils::signal<void(System*, const std::string&)>& battleback1NameModified();
  rpgmutils::signal<void(System*, const std::string&)>& battleback2NameModified();
  rpgmutils::signal<void(System*, int)>& battlerHueModified();
  rpgmutils::signal<void(System*, const std::string&)>& battlerNameModified();
  rpgmutils::signal<void(System*, const Vehicle&)>& boatModified();
  rpgmutils::signal<void(System*, const std::string&)>& currencyUnitModified();
  rpgmutils::signal<void(System*, const Audio&)>& defeatMeModified();
  rpgmutils::signal<void(System*, int)>& editMapIdModified();
  rpgmutils::signal<void(System*, const std::vector<std::string>&)>& elementsModified();
  rpgmutils::signal<void(System*, const std::vector<std::string>&)>& equipTypesModified();
  rpgmutils::signal<void(System*, const std::string&)>& gameTitleModified();
  rpgmutils::signal<void(System*, const Audio&)>& gameoverMeModified();
  rpgmutils::signal<void(System*, const std::string&)>& localeModified();
  rpgmutils::signal<void(System*, const std::vector<int>&)>& magicSkillsModified();
  rpgmutils::signal<void(System*, const std::array<bool, 6>&)>& menuCommandsModified();
  rpgmutils::signal<void(System*, bool)>& optDisplayTpModified();
  rpgmutils::signal<void(System*, bool)>& optDrawTitleModified();
  rpgmutils::signal<void(System*, bool)>& optExtraExpModified();
  rpgmutils::signal<void(System*, bool)>& optFloorDeathModified();
  rpgmutils::signal<void(System*, bool)>& optFollowersModified();
  rpgmutils::signal<void(System*, bool)>& optSideViewModified();
  rpgmutils::signal<void(System*, bool)>& optSlipDeathModified();
  rpgmutils::signal<void(System*, bool)>& optTransparentModified();
  rpgmutils::signal<void(System*, const std::vector<int>&)>& partyMembersModified();
  rpgmutils::signal<void(System*, const Vehicle&)>& shipModified();
  rpgmutils::signal<void(System*, const std::vector<std::string>&)>& skillTypesModified();
  rpgmutils::signal<void(System*, const std::vector<Audio>&)>& soundsModified();
  rpgmutils::signal<void(System*, int)>& startMapIdModified();
  rpgmutils::signal<void(System*, int)>& startXModified();
  rpgmutils::signal<void(System*, int)>& startYModified();
  rpgmutils::signal<void(System*, const std::vector<std::string>&)>& switchesModified();
  rpgmutils::signal<void(System*, const Terms&)>& termsModified();
  rpgmutils::signal<void(System*, const std::vector<Battler>&)>& testBattlersModified();
  rpgmutils::signal<void(System*, int)>& testTroopIdModified();
  rpgmutils::signal<void(System*, const std::string&)>& title1NameModified();
  rpgmutils::signal<void(System*, const std::string&)>& title2NameModified();
  rpgmutils::signal<void(System*, const Audio&)>& titleBgmModified();
  rpgmutils::signal<void(System*, const std::vector<std::string>&)>& variablesModified();
  rpgmutils::signal<void(System*, int)>& tileSizeModified();
  rpgmutils::signal<void(System*, uint32_t)>& versionIdModified();
  rpgmutils::signal<void(System*, const Audio&)>& victoryMeModified();
  rpgmutils::signal<void(System*, const std::vector<std::string>&)>& weaponTypesModified();
  rpgmutils::signal<void(System*, const std::array<int, 3>&)>& windowToneModified();
  rpgmutils::signal<void(System*, bool)>& hasEncryptedImagesModified();
  rpgmutils::signal<void(System*, bool)>& hasEncryptedAudioModified();
  rpgmutils::signal<void(System*, const std::string&)>& encryptionKeyModified();

private:
  Vehicle m_airship;
  std::vector<std::string> m_armorTypes;
  std::vector<Motion> m_attackMotions;
  Audio m_battleBgm;
  std::string m_battleback1Name;
  std::string m_battleback2Name;
  int m_battlerHue{};
  std::string m_battlerName;
  Vehicle m_boat;
  std::string m_currencyUnit;
  Audio m_defeatMe;
  int m_editMapId{};
  std::vector<std::string> m_elements;
  std::vector<std::string> m_equipTypes;
  std::string m_gameTitle;
  Audio m_gameoverMe;
  std::string m_locale;
  std::vector<int> m_magicSkills;
  std::array<bool, 6> m_menuCommands{}; // Item, Skill, Equip, Status, Formation, Save;
  bool m_optDisplayTp{};
  bool m_optDrawTitle{};
  bool m_optExtraExp{};
  bool m_optFloorDeath{};
  bool m_optFollowers{};
  bool m_optSideView{};
  bool m_optSlipDeath{};
  bool m_optTransparent{};
  std::vector<int> m_partyMembers;
  Vehicle m_ship;
  std::vector<std::string> m_skillTypes;
  std::vector<Audio> m_sounds;
  int m_startMapId{};
  int m_startX{};
  int m_startY{};
  std::vector<std::string> m_switches;
  Terms m_terms;
  std::vector<Battler> m_testBattlers;
  int m_testTroopId{};
  std::string m_title1Name;
  std::string m_title2Name;
  Audio m_titleBgm;
  std::vector<std::string> m_variables;
  int m_tileSize{48};
  uint32_t m_versionId = 0;
  Audio m_victoryMe;
  std::vector<std::string> m_weaponTypes;
  std::array<int, 3> m_windowTone;
  std::optional<bool> m_hasEncryptedImages;
  std::optional<bool> m_hasEncryptedAudio;
  std::optional<std::string> m_encryptionKey;

  std::optional<Vehicle> m_oldairship;
  std::optional<std::vector<std::string>> m_oldarmorTypes;
  std::optional<std::vector<Motion>> m_oldattackMotions;
  std::optional<Audio> m_oldbattleBgm;
  std::optional<std::string> m_oldbattleback1Name;
  std::optional<std::string> m_oldbattleback2Name;
  std::optional<int> m_oldbattlerHue{};
  std::optional<std::string> m_oldbattlerName;
  std::optional<Vehicle> m_oldboat;
  std::optional<std::string> m_oldcurrencyUnit;
  std::optional<Audio> m_olddefeatMe;
  std::optional<int> m_oldeditMapId{};
  std::optional<std::vector<std::string>> m_oldelements;
  std::optional<std::vector<std::string>> m_oldequipTypes;
  std::optional<std::string> m_oldgameTitle;
  std::optional<Audio> m_oldgameoverMe;
  std::optional<std::string> m_oldlocale;
  std::optional<std::vector<int>> m_oldmagicSkills;
  std::optional<std::array<bool, 6>> m_oldmenuCommands{};
  std::optional<bool> m_oldoptDisplayTp{};
  std::optional<bool> m_oldoptDrawTitle{};
  std::optional<bool> m_oldoptExtraExp{};
  std::optional<bool> m_oldoptFloorDeath{};
  std::optional<bool> m_oldoptFollowers{};
  std::optional<bool> m_oldoptSideView{};
  std::optional<bool> m_oldoptSlipDeath{};
  std::optional<bool> m_oldoptTransparent{};
  std::optional<std::vector<int>> m_oldpartyMembers;
  std::optional<Vehicle> m_oldship;
  std::optional<std::vector<std::string>> m_oldskillTypes;
  std::optional<std::vector<Audio>> m_oldsounds;
  std::optional<int> m_oldstartMapId{};
  std::optional<int> m_oldstartX{};
  std::optional<int> m_oldstartY{};
  std::optional<std::vector<std::string>> m_oldswitches;
  std::optional<Terms> m_oldterms;
  std::optional<std::vector<Battler>> m_oldtestBattlers;
  std::optional<int> m_oldtestTroopId{};
  std::optional<std::string> m_oldtitle1Name;
  std::optional<std::string> m_oldtitle2Name;
  std::optional<Audio> m_oldtitleBgm;
  std::optional<std::vector<std::string>> m_oldvariables;
  std::optional<int> m_oldtileSize{48};
  std::optional<uint32_t> m_oldversionId = 0;
  std::optional<Audio> m_oldvictoryMe;
  std::optional<std::vector<std::string>> m_oldweaponTypes;
  std::optional<std::array<int, 3>> m_oldwindowTone;
  std::optional<bool> m_oldhasEncryptedImages;
  std::optional<bool> m_oldhasEncryptedAudio;
  std::optional<std::string> m_oldencryptionKey;

  std::optional<rpgmutils::signal<void(System*, const Vehicle&)>> m_airshipModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<std::string>&)>> m_armorTypesModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<Motion>&)>> m_attackMotionsModified;
  std::optional<rpgmutils::signal<void(System*, const Audio&)>> m_battleBgmModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_battleback1NameModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_battleback2NameModified;
  std::optional<rpgmutils::signal<void(System*, int)>> m_battlerHueModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_battlerNameModified;
  std::optional<rpgmutils::signal<void(System*, const Vehicle&)>> m_boatModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_currencyUnitModified;
  std::optional<rpgmutils::signal<void(System*, const Audio&)>> m_defeatMeModified;
  std::optional<rpgmutils::signal<void(System*, int)>> m_editMapIdModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<std::string>&)>> m_elementsModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<std::string>&)>> m_equipTypesModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_gameTitleModified;
  std::optional<rpgmutils::signal<void(System*, const Audio&)>> m_gameoverMeModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_localeModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<int>&)>> m_magicSkillsModified;
  std::optional<rpgmutils::signal<void(System*, const std::array<bool, 6>&)>> m_menuCommandsModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optDisplayTpModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optDrawTitleModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optExtraExpModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optFloorDeathModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optFollowersModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optSideViewModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optSlipDeathModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_optTransparentModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<int>&)>> m_partyMembersModified;
  std::optional<rpgmutils::signal<void(System*, const Vehicle&)>> m_shipModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<std::string>&)>> m_skillTypesModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<Audio>&)>> m_soundsModified;
  std::optional<rpgmutils::signal<void(System*, int)>> m_startMapIdModified;
  std::optional<rpgmutils::signal<void(System*, int)>> m_startXModified;
  std::optional<rpgmutils::signal<void(System*, int)>> m_startYModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<std::string>&)>> m_switchesModified;
  std::optional<rpgmutils::signal<void(System*, const Terms&)>> m_termsModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<Battler>&)>> m_testBattlersModified;
  std::optional<rpgmutils::signal<void(System*, int)>> m_testTroopIdModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_title1NameModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_title2NameModified;
  std::optional<rpgmutils::signal<void(System*, const Audio&)>> m_titleBgmModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<std::string>&)>> m_variablesModified;
  std::optional<rpgmutils::signal<void(System*, int)>> m_tileSizeModified;
  std::optional<rpgmutils::signal<void(System*, uint32_t)>> m_versionIdModified;
  std::optional<rpgmutils::signal<void(System*, const Audio&)>> m_victoryMeModified;
  std::optional<rpgmutils::signal<void(System*, const std::vector<std::string>&)>> m_weaponTypesModified;
  std::optional<rpgmutils::signal<void(System*, const std::array<int, 3>&)>> m_windowToneModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_hasEncryptedImagesModified;
  std::optional<rpgmutils::signal<void(System*, bool)>> m_hasEncryptedAudioModified;
  std::optional<rpgmutils::signal<void(System*, const std::string&)>> m_encryptionKeyModified;
};

void to_json(nlohmann::ordered_json& j, const System::Motion& motion);
void from_json(const nlohmann::ordered_json& j, System::Motion& motion);
void to_json(nlohmann::ordered_json& j, const System& system);
void from_json(const nlohmann::ordered_json& j, System& system);