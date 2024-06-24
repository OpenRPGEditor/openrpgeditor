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
  int d_source;
  int d_raw_source = 1;
  int d_value = 0;
  constexpr auto &actorSourceNames = magic_enum::enum_names<ActorDataSource>();
  constexpr auto &enemySourceNames = magic_enum::enum_names<EnemyDataSource>();
  constexpr auto &characterSourceNames = magic_enum::enum_names<CharacterDataSource>();
  constexpr auto &otherSourceNames = magic_enum::enum_names<OtherDataSource>();
  char current_actorSource;
  char current_enemySource;
  char current_otherSource;
  int current_partySource = 1;
  int current_characterSource = 0;
  std::optional<ControlVariables> command;
  std::optional<ObjectPicker<Item>> i_picker;
  std::optional<ObjectPicker<Actor>> a_picker;
  std::optional<ObjectPicker<Armor>> ar_picker;
  std::optional<ObjectPicker<Weapon>> w_picker;
  std::optional<ObjectPicker<Enemy>> e_picker;
  std::tuple<bool, bool> result;
};