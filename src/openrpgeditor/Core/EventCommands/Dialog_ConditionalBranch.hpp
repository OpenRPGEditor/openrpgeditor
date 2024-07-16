#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/ConditionalBranch.hpp"
#include "Database/Actors.hpp"
#include "Database/Weapons.hpp"
#include "Database/Armors.hpp"
#include "Database/Skills.hpp"
#include "Database/States.hpp"
#include "Database/Classes.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"

struct Project;
struct Dialog_ConditionalBranch : IEventDialogController {
  Dialog_ConditionalBranch() = delete;
  explicit Dialog_ConditionalBranch(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ConditionalBranchCommand());
  }
  std::tuple<bool, bool> draw() override;

  Project* m_project = nullptr;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_conditionType{0};
  int m_switch_id{1};
  int m_switch_value{0};
  int m_picker_type;
  int m_variable_id{1};
  int m_variable_value{0};
  int m_variable_subSource{0};
  int m_sub_constant{0};
  int m_sub_variable_id{1};
  std::string m_selfSw{"A"};
  int m_selfSw_value{0};
  int m_timer_min{0};
  int m_timer_sec{0};
  int m_timer_operation{0};
  int m_actor_selection{0};
  int m_actor_sub_selection{0};
  std::string m_actor_selection_nameinput;
  int m_actor_class{1};
  int m_actor_skill{1};
  int m_actor_armor{1};
  int m_actor_weapon{1};
  int m_actor_state{1};

  bool m_elseBranch{false};

  std::shared_ptr<ConditionalBranchCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ObjectPicker<State>> state_picker;
  std::optional<ObjectPicker<Weapon>> weapon_picker;
  std::optional<ObjectPicker<Armor>> armor_picker;
  std::optional<ObjectPicker<Skill>> skill_picker;
  std::optional<ObjectPicker<Class>> class_picker;
  std::tuple<bool, bool> result;
};