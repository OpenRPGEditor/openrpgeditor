#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/GameConstants.hpp"

#include <optional>

struct Actor;
struct Armor;
struct Class;
struct Skill;
struct Item;
struct Weapon;
struct Enemy;
struct Troop;
struct State;
struct Animation;
struct Tileset;
struct CommonEvent;
struct MapInfo;
struct GameConstants;
struct DBGameConstantsTab : IDBEditorTab {
  explicit DBGameConstantsTab(GameConstants& constants, DatabaseEditor* parent) : IDBEditorTab(parent), m_constants(&constants) {}

  void draw() override;

private:
  void drawAliasModal(GameConstants::Type type);
  void setupTableHeaders();
  bool drawSelectable(int id, bool selected);
  void drawNameAndAliasColumns(const std::string& name, const std::string& alias);
  bool drawDeleteButton(int id);

  GameConstants* m_constants = nullptr;
  std::optional<VariableSwitchPicker> m_switchVariblePicker;
  std::optional<ObjectPicker<Actor>> m_actorsPicker;
  std::optional<ObjectPicker<Class>> m_classesPicker;
  std::optional<ObjectPicker<Skill>> m_skillsPicker;
  std::optional<ObjectPicker<Item>> m_itemsPicker;
  std::optional<ObjectPicker<Weapon>> m_weaponsPicker;
  std::optional<ObjectPicker<Armor>> m_armorsPicker;
  std::optional<ObjectPicker<Enemy>> m_enemiesPicker;
  std::optional<ObjectPicker<Troop>> m_troopsPicker;
  std::optional<ObjectPicker<State>> m_statesPicker;
  std::optional<ObjectPicker<Animation>> m_animationPicker;
  std::optional<ObjectPicker<Tileset>> m_tilesetPicker;
  std::optional<ObjectPicker<std::optional<CommonEvent>>> m_commonEventPicker;
  std::optional<ObjectPicker<std::optional<MapInfo>>> m_mapsPicker;

  int m_selection = -1;
  std::optional<std::string> m_currentAlias;
  bool m_openPopup = false;
};
