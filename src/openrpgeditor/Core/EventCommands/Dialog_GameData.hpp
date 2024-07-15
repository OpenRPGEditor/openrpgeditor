#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Project.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Items.hpp"
#include "Database/EventCommands/ControlVariables.hpp"

struct Project;
struct Dialog_GameData : IEventDialogController {
  Dialog_GameData() = delete;
  explicit Dialog_GameData(const std::string& name, Project* project, int type, int rawSource, int value = 0) : IEventDialogController(name), m_project(project), m_type(type), m_raw_source(rawSource), m_value(value) {
    command.gameData.value = 0;
  }
  std::tuple<bool, bool> draw() override;

  ControlVariables& getData() { return command; }
  const ControlVariables& getData() const { return command; }


  Project* m_project = nullptr;

private:
  bool m_confirmed{false};

  int m_type;
  int m_raw_source; // Final value
  int m_value;

  int m_weapon_source{1};
  int m_armor_source{1};
  int m_item_source{1};
  int m_actor_source{1};

  static constexpr auto ActorData = magic_enum::enum_values<ActorDataSource>();
  static constexpr auto EnemyData = magic_enum::enum_values<EnemyDataSource>();
  static constexpr auto CharacterData = magic_enum::enum_values<CharacterDataSource>();
  static constexpr auto OtherSource = magic_enum::enum_values<OtherDataSource>();

  int current_otherSource = 0;
  int current_partySource = 1;
  int current_characterSource = -1;
  int current_enemySource = 1;

  int current_actorDataSource = 0;
  int current_enemyDataSource = 0;
  int current_characterDataSource = 0;

  ControlVariables command;
  std::optional<ObjectPicker<Item>> i_picker;
  std::optional<ObjectPicker<Actor>> a_picker;
  std::optional<ObjectPicker<Armor>> ar_picker;
  std::optional<ObjectPicker<Weapon>> w_picker;
  std::tuple<bool, bool> result;
};