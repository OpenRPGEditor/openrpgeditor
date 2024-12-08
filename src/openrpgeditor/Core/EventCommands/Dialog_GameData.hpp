#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/ControlVariables.hpp"
#include "Database/Items.hpp"

struct Dialog_GameData : IEventDialogController {
  Dialog_GameData() = delete;
  explicit Dialog_GameData(const std::string& name, int type, int rawSource, int value = 0) : IEventDialogController(name), m_type(type), m_raw_source(rawSource), m_value(value) {
    command.gameData.value = 0;
  }
  std::tuple<bool, bool> draw() override;

  ControlVariables& getData() { return command; }
  const ControlVariables& getData() const { return command; }

private:
  bool m_confirmed{false};

  int m_type;
  int m_raw_source; // Final value
  int m_value;

  int m_weaponSource{1};
  int m_armorSource{1};
  int m_itemSource{1};
  int m_actorSource{1};

  static constexpr auto ActorData = magic_enum::enum_values<ActorDataSource>();
  static constexpr auto EnemyData = magic_enum::enum_values<EnemyDataSource>();
  static constexpr auto CharacterData = magic_enum::enum_values<CharacterDataSource>();
  static constexpr auto OtherSource = magic_enum::enum_values<OtherDataSource>();

  int current_otherSource = 0;
  int current_partySource = 1;
  int current_characterSource = -1;
  int m_currentEnemySource = 1;

  int m_currentActorDataSource = 0;
  int m_currentEnemyDataSource = 0;
  int current_characterDataSource = 0;

  ControlVariables command;
  std::optional<ObjectPicker<Item>> m_itemPicker;
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<Armor>> m_armorPicker;
  std::optional<ObjectPicker<Weapon>> m_weaponPicker;
  std::tuple<bool, bool> result;
};