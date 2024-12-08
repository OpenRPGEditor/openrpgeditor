#pragma once

#include "Database/IModifiable.hpp"
#include "Database/Trait.hpp"

#include <nlohmann/json.hpp>
#include <vector>

class Actor final : public IModifiable {
  friend class ActorsSerializer;
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

  /* Signals */
  signal<void(Actor* actor, int newId)>& onIdModified();
  signal<void(Actor* actor, const std::string& name)>& onBattlerNameModified();
  signal<void(Actor* actor, int newIndex)>& onCharacterIndexModified();
  signal<void(Actor* actor, const std::string& name)>& onCharacterNameModified();
  signal<void(Actor* actor, int newId)>& onClassIdModified();
  signal<void(Actor* actor, const std::vector<int>& newEquips)>& onEquipsModified();
  signal<void(Actor* actor, int index, int newEquip)>& onEquipAdded();
  signal<void(Actor* actor, int index, int newEquip)>& onEquipModified();
  signal<void(Actor* actor, int index, int newEquip)>& onEquipRemoved();
  signal<void(Actor* actor, int newId)>& onFaceIndexModified();
  signal<void(Actor* actor, const std::string& newFaceName)>& onFaceNameModified();
  signal<void(Actor* actor, const std::vector<Trait>& newTraits)>& onTraitsModified();
  signal<void(Actor* actor, const Trait& trait)>& onTraitAdded();
  signal<void(Actor* actor, int newInitialLevel)>& onInitialLevelModified();
  signal<void(Actor* actor, int newMaxLevel)>& onMaxLevelModified();
  signal<void(Actor* actor, const std::string& newName)>& onNameModified();
  signal<void(Actor* actor, const std::string& newNickname)>& onNicknameModified();
  signal<void(Actor* actor, const std::string& newNote)>& onNoteModified();
  signal<void(Actor* actor, const std::string& newProfile)>& onProfileModified();

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

  /* Signals */
  std::optional<signal<void(Actor* actor, int newId)>> m_onIdModified;
  std::optional<signal<void(Actor* actor, const std::string& newName)>> m_onBattlerNameModified;
  std::optional<signal<void(Actor* actor, int newIndex)>> m_onCharacterIndexModified;
  std::optional<signal<void(Actor* actor, const std::string& newName)>> m_onCharacterNameModified;
  std::optional<signal<void(Actor* actor, int newId)>> m_onClassIdModified;
  std::optional<signal<void(Actor* actor, const std::vector<int>& newEquips)>> m_onEquipsModified;
  std::optional<signal<void(Actor* actor, int index, int newEquip)>> m_onEquipAdded;
  std::optional<signal<void(Actor* actor, int index, int newEquip)>> m_onEquipModified;
  std::optional<signal<void(Actor* actor, int index, int newEquip)>> m_onEquipRemoved;
  std::optional<signal<void(Actor* actor, int newIndex)>> m_onFaceIndexModified;
  std::optional<signal<void(Actor* actor, const std::string& newFaceName)>> m_onFaceNameModified;
  std::optional<signal<void(Actor* actor, const std::vector<Trait>& newTraits)>> m_onTraitsModified;
  std::optional<signal<void(Actor* actor, const Trait& trait)>> m_onTraitAdded;
  std::optional<signal<void(Actor* actor, int newInitialLevel)>> m_onInitialLevelModified;
  std::optional<signal<void(Actor* actor, int newMaxLevel)>> m_onMaxLevelModified;
  std::optional<signal<void(Actor* actor, const std::string& newName)>> m_onNameModified;
  std::optional<signal<void(Actor* actor, const std::string& newNickname)>> m_onNicknameModified;
  std::optional<signal<void(Actor* actor, const std::string& newNote)>> m_onNoteModified;
  std::optional<signal<void(Actor* actor, const std::string& newProfile)>> m_onProfileModified;

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