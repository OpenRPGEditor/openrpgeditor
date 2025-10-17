#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/DatabaseEditor/IDBEditorTab.hpp"
#include "Database/GameConstants.hpp"
#include "DBCommonEventsTab.hpp"

#include <optional>

class Actor;
class Armor;
class Class;
class Skill;
class Item;
class Weapon;
class Enemy;
class Troop;
class State;
class Animation;
class Tileset;
class CommonEvent;
class MapInfo;
struct GameConstants;
class DBGameConstantsTab final : public IDBEditorTab {
public:
  void draw() override;

  [[nodiscard]] std::string tabName() const override { return tr("Game Constants"); }
  [[nodiscard]] constexpr std::string_view tabId() const override { return "##DBGameConstantsTab"sv; };

  bool isReady() const override {
    return Database::instance()->actors && Database::instance()->classes && Database::instance()->skills && Database::instance()->items && Database::instance()->weapons &&
           Database::instance()->armors && Database::instance()->enemies && Database::instance()->troops && Database::instance()->states && Database::instance()->animations &&
           Database::instance()->animations && Database::instance()->tilesets && Database::instance()->commonEvents && Database::instance()->system && Database::instance()->plugins &&
           Database::instance()->mapInfos && Database::instance()->gameConstants;
  }

  bool isInitialized() const override { return m_constants; }

  void initialize() override {
    if (!isReady()) {
      return;
    }

    m_constants = &Database::instance()->gameConstants.value();
  }

private:
  void drawAliasModal(GameConstants::Type type);
  static void setupTableHeaders();
  bool drawSelectable(int id, bool selected);
  static void drawNameAndAliasColumns(const std::string& name, const std::string& alias);
  static bool drawDeleteButton(int id);

  GameConstants* m_constants = nullptr;
  std::optional<VariableSwitchPicker> m_switchVariablePicker;
  std::optional<ActorPicker> m_actorsPicker;
  std::optional<ClassPicker> m_classesPicker;
  std::optional<SkillPicker> m_skillsPicker;
  std::optional<ItemPicker> m_itemsPicker;
  std::optional<WeaponPicker> m_weaponsPicker;
  std::optional<ArmorPicker> m_armorsPicker;
  std::optional<EnemyPicker> m_enemiesPicker;
  std::optional<TroopPicker> m_troopsPicker;
  std::optional<StatePicker> m_statesPicker;
  std::optional<AnimationPicker> m_animationPicker;
  std::optional<TilesetPicker> m_tilesetPicker;
  std::optional<CommonEventPicker> m_commonEventPicker;
  std::optional<MapInfoPicker> m_mapsPicker;

  int m_selection = -1;
  std::optional<std::string> m_currentAlias;
  bool m_openPopup = false;
};
