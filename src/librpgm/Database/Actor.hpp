#pragma once

#include "Database/IModifiable.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <vector>

class Actor final : public IModifiable {
  friend void to_json(nlohmann::ordered_json& to, const Actor& actor);
  friend void from_json(const nlohmann::ordered_json& from, Actor& actor);

public:
  Actor() = default;
  Actor(const Actor& other);
  Actor& operator=(const Actor& other);
  Actor(Actor&& other) noexcept;
  Actor& operator=(Actor&& other) noexcept;

  /**
   *
   * @return
   */
  int id() const;

  /**
   *
   * @param id
   */
  void setId(int id);

  /**
   *
   * @return
   */
  const std::string& battlerName() const;

  /**
   *
   * @param battlerName
   */
  void setBattlerName(const std::string& battlerName);

  /**
   *
   * @return
   */
  int characterIndex() const;

  /**
   *
   * @param characterIndex
   */
  void setCharacterIndex(int characterIndex);

  /**
   *
   * @return
   */
  const std::string& characterName() const;

  /**
   *
   * @param characterName
   */
  void setCharacterName(const std::string& characterName);

  /**
   *
   * @return
   */

  int classId() const;
  /**
   *
   * @param classId
   */
  void setClassId(int classId);

  /**
   *
   * @return
   */
  const std::vector<int>& equips() const;

  /**
   *
   * @param equips
   */
  void setEquips(const std::vector<int>& equips);

  /**
   *
   * @param equip
   */
  void addEquip(int equip);

  /**
   *
   * @param index
   * @param equip
   */
  void setEquip(int index, int equip);

  /**
   *
   * @param index
   */
  void removeEquip(int index);

  /**
   *
   * @return
   */
  int faceIndex() const;

  /**
   *
   * @param faceIndex
   */
  void setFaceIndex(int faceIndex);

  /**
   *
   * @return
   */
  const std::string& faceName() const;

  /**
   *
   * @param faceName
   */
  void setFaceName(const std::string& faceName);

  /**
   *
   * @return
   */
  std::vector<Trait>& traits();

  /**
   *
   * @param traits
   */
  void setTraits(const std::vector<Trait>& traits);

  /**
   *
   * @param index
   * @return
   */
  Trait& trait(int index);

  /**
   *
   * @return
   */
  int initialLevel() const;
  /**
   *
   * @param initialLevel
   */
  void setInitialLevel(int initialLevel);

  /**
   *
   * @return
   */
  int maxLevel() const;

  /**
   *
   * @param maxLevel
   */
  void setMaxLevel(int maxLevel);

  /**
   *
   * @return
   */
  const std::string& name() const;

  /**
   *
   * @param name
   */
  void setName(const std::string& name);

  /**
   *
   * @return
   */
  const std::string& nickname() const;

  /**
   *
   * @param nickname
   */
  void setNickname(const std::string& nickname);

  /**
   *
   * @return
   */
  const std::string& note() const;
  /**
   *
   * @param note
   */
  void setNote(const std::string& note);

  /**
   *
   * @return
   */
  const std::string& profile() const;
  /**
   *
   * @param profile
   */
  void setProfile(const std::string& profile);

  /**
   *
   * @return
   */
  bool isValid() const;
  /**
   *
   * @param valid
   */
  void setValid(bool valid);

  void restoreOriginal() override;
  void acceptChanges() override;
  nlohmann::ordered_json serializeOldValues() const override;

  /* Signals */
  rpgmutils::signal<void(Actor* actor, int newId)>& idModified();
  rpgmutils::signal<void(Actor* actor, const std::string& name)>& battlerNameModified();
  rpgmutils::signal<void(Actor* actor, int newIndex)>& characterIndexModified();
  rpgmutils::signal<void(Actor* actor, const std::string& name)>& characterNameModified();
  rpgmutils::signal<void(Actor* actor, int newId)>& classIdModified();
  rpgmutils::signal<void(Actor* actor, const std::vector<int>& newEquips)>& equipsModified();
  rpgmutils::signal<void(Actor* actor, int index, int newEquip)>& equipAdded();
  rpgmutils::signal<void(Actor* actor, int index, int newEquip)>& equipModified();
  rpgmutils::signal<void(Actor* actor, int index, int newEquip)>& equipRemoved();
  rpgmutils::signal<void(Actor* actor, int newId)>& faceIndexModified();
  rpgmutils::signal<void(Actor* actor, const std::string& newFaceName)>& faceNameModified();
  rpgmutils::signal<void(Actor* actor, const std::vector<Trait>& newTraits)>& traitsModified();
  rpgmutils::signal<void(Actor* actor, const Trait& trait)>& traitAdded();
  rpgmutils::signal<void(Actor* actor, int newInitialLevel)>& initialLevelModified();
  rpgmutils::signal<void(Actor* actor, int newMaxLevel)>& maxLevelModified();
  rpgmutils::signal<void(Actor* actor, const std::string& newName)>& nameModified();
  rpgmutils::signal<void(Actor* actor, const std::string& newNickname)>& nicknameModified();
  rpgmutils::signal<void(Actor* actor, const std::string& newNote)>& noteModified();
  rpgmutils::signal<void(Actor* actor, const std::string& newProfile)>& profileModified();

  bool isModified() const override {
    return (IModifiable::isModified() | std::ranges::any_of(m_traits, [](const Trait& trait) { return trait.isModified(); }));
  }

private:
  int m_id = 1;
  std::string m_battlerName;
  int m_characterIndex = 0;
  std::string m_characterName;
  int m_classId = 1;
  std::vector<int> m_equips;
  int m_faceIndex = 0;
  std::string m_faceName;
  std::vector<Trait> m_traits;
  int m_initialLevel = 1;
  int m_maxLevel = 99;
  std::string m_name;
  std::string m_nickname;
  std::string m_note;
  std::string m_profile;

  // !!!DO NOT CHANGE CASING!!!
  std::optional<int> m_oldid;
  std::optional<std::string> m_oldbattlerName;
  std::optional<int> m_oldcharacterIndex;
  std::optional<std::string> m_oldcharacterName;
  std::optional<int> m_oldclassId;
  std::optional<std::vector<int>> m_oldequips;
  std::optional<int> m_oldfaceIndex;
  std::optional<std::string> m_oldfaceName;
  std::optional<std::vector<Trait>> m_oldtraits;
  std::optional<int> m_oldinitialLevel;
  std::optional<int> m_oldmaxLevel;
  std::optional<std::string> m_oldname;
  std::optional<std::string> m_oldnickname;
  std::optional<std::string> m_oldnote;
  std::optional<std::string> m_oldprofile;

  /* Signals */
  std::optional<rpgmutils::signal<void(Actor* actor, int newId)>> m_idModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::string& newName)>> m_battlerNameModified;
  std::optional<rpgmutils::signal<void(Actor* actor, int newIndex)>> m_characterIndexModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::string& newName)>> m_characterNameModified;
  std::optional<rpgmutils::signal<void(Actor* actor, int newId)>> m_onClassIdModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::vector<int>& newEquips)>> m_equipsModified;
  std::optional<rpgmutils::signal<void(Actor* actor, int index, int newEquip)>> m_equipAdded;
  std::optional<rpgmutils::signal<void(Actor* actor, int index, int newEquip)>> m_equipModified;
  std::optional<rpgmutils::signal<void(Actor* actor, int index, int newEquip)>> m_equipRemoved;
  std::optional<rpgmutils::signal<void(Actor* actor, int newIndex)>> m_faceIndexModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::string& newFaceName)>> m_faceNameModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::vector<Trait>& newTraits)>> m_traitsModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const Trait& trait)>> m_traitAdded;
  std::optional<rpgmutils::signal<void(Actor* actor, int newInitialLevel)>> m_initialLevelModified;
  std::optional<rpgmutils::signal<void(Actor* actor, int newMaxLevel)>> m_maxLevelModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::string& newName)>> m_nameModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::string& newNickname)>> m_nicknameModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::string& newNote)>> m_noteModified;
  std::optional<rpgmutils::signal<void(Actor* actor, const std::string& newProfile)>> m_profileModified;

  /*!
   * @name m_isValid
   * @details
   * Indicates that this is an actual valid entry and not a dummy
   *
   * When making a new entry make sure to set this to true or it won't be
   * serialized.
   */
  bool m_isValid{false};
};
void to_json(nlohmann::json& to, const Actor& actor);
void from_json(const nlohmann::json& from, Actor& actor);