#pragma once
#include "IDialogController.hpp"
#include "Core/Project.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Enemies.hpp"
#include "Database/Items.hpp"
#include "Database/EventCommands/ControlVariables.hpp"

struct Project;
struct Dialog_GameData : IDialogController {
  Dialog_GameData() = delete;
  explicit Dialog_GameData(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::optional<ControlVariables> getCommandData() { return command; }
  std::string getUIString() {

    if (!m_confirmed)
      return "";

    if (command->gameData.source == GameDataSource::Item) {
      return std::string("The number of " + formatString(m_project->item(command->gameData.rawSource)->name, command->gameData.rawSource));
    }
    if (command->gameData.source == GameDataSource::Weapon) {
      return std::string("The number of " + formatString(m_project->weapon(command->gameData.rawSource)->name, command->gameData.rawSource));
    }
    if (command->gameData.source == GameDataSource::Armor) {
      return std::string("The number of " + formatString(m_project->armor(command->gameData.rawSource)->name, command->gameData.rawSource));
    }
    if (command->gameData.source == GameDataSource::Actor) {
      return std::string(magic_enum::enum_name(static_cast<ActorDataSource>(command->gameData.value)).data()) + " of " + formatString(m_project->actor(command->gameData.rawSource)->name, command->gameData.rawSource);
    }
    if (command->gameData.source == GameDataSource::Enemy) {
      return std::string(magic_enum::enum_name(static_cast<EnemyDataSource>(command->gameData.value)).data()) + " of " + formatString(m_project->enemy(command->gameData.rawSource)->name, command->gameData.rawSource);
    }
    if (command->gameData.source == GameDataSource::Character) {
      std::string str = command->gameData.rawSource == -1 ? "Player" : command->gameData.rawSource == 0 ? "This Event" : m_project->event(command->gameData.rawSource)->name;
      return std::string(magic_enum::enum_name(static_cast<CharacterDataSource>(command->gameData.value)).data()) + " of " + formatString(str, command->gameData.rawSource);
    }
    if (command->gameData.source == GameDataSource::Party) {
      return "Actor ID of the party member #" + std::to_string(command->gameData.rawSource);
    }
    if (command->gameData.source == GameDataSource::Other) {
      return std::string(magic_enum::enum_name(static_cast<OtherDataSource>(command->gameData.value)).data());
    }
    return "";
  }
  Project* m_project = nullptr;
private:
  bool m_confirmed{false};

  int d_source = 0;
  int d_raw_source = 1; // Final value
  int d_value = 0;

  int d_weapon_source = 1;
  int d_armor_source = 1;
  int d_item_source = 1;
  int d_actor_source = 1;

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
  std::tuple<bool, bool> result;

  static std::string formatString(std::string str, int source) {
    if (str == "") {
      return std::format("#{:04} ", source);
    }
    else {
      return str;
    }
  }
};