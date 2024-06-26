#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/CommandParser.hpp"
#include "Database/Enemies.hpp"
#include "Database/Items.hpp"
struct Project;
struct Dialog_GameData : IDialogController {
  Dialog_GameData() = delete;
  explicit Dialog_GameData(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  void draw() override;

  Project* m_project = nullptr;
private:
  int d_source = 0;
  int d_raw_source = 1;
  int d_value = 0;

   static constexpr auto ActorData = magic_enum::enum_values<ActorDataSource>();
   static constexpr auto EnemyData = magic_enum::enum_values<EnemyDataSource>();
   static constexpr auto CharacterData = magic_enum::enum_values<CharacterDataSource>();
   static constexpr auto OtherSource = magic_enum::enum_values<OtherDataSource>();

  int current_otherSource = 0;
  int current_partySource = 0;
  int current_characterSource = 0;
  int current_enemySource = 0;

  int current_actorDataSource = 0;
  int current_enemyDataSource = 0;
  int current_characterDataSource = 0;

  std::optional<ControlVariables> command;
  std::optional<ObjectPicker<Item>> i_picker;
  std::optional<ObjectPicker<Actor>> a_picker;
  std::optional<ObjectPicker<Armor>> ar_picker;
  std::optional<ObjectPicker<Weapon>> w_picker;
  std::optional<ObjectPicker<Enemy>> e_picker;
  std::tuple<bool, bool> result;
};