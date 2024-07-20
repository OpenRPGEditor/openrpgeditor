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
#include "Database/Items.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/EventCommands/End.hpp"

struct Project;
struct Dialog_ConditionalBranch : IEventDialogController {
  Dialog_ConditionalBranch() = delete;
  explicit Dialog_ConditionalBranch(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ConditionalBranchCommand());

    if (command->type == ConditionType::Switch) {
      m_switch_id = command->globalSwitch.switchIdx;
      m_switch_value = static_cast<int>(command->globalSwitch.checkIfOn);
    }
    else if (command->type == ConditionType::Variable) {
      m_variable_id = command->variable.id;
      m_variable_subSource = static_cast<int>(command->variable.source);
      m_variable_value = static_cast<int>(command->variable.comparison);
      if (command->variable.source == VariableComparisonSource::Constant) {
         m_sub_constant = command->variable.constant;
      }
      else {
        m_sub_variable_id = command->variable.otherId;
      }
    }
    else if (command->type == ConditionType::Self_Switch) {
      m_selfSw = command->selfSw;
      m_selfSw_value = static_cast<int>(command->selfSwitch.checkIfOn);
    }
    else if (command->type == ConditionType::Timer) {
      //command->timer->sec = m_timer_sec + (m_timer_min * 60);
      m_timer_operation = static_cast<int>(command->timer.comparison);
    }
    else if (command->type == ConditionType::Actor) {
      m_actor_selection = command->actor.id;
      m_actor_sub_selection = static_cast<int>(command->actor.type);
      if (command->actor.type == ActorConditionType::In_The_Party) {
        //
      }
      if (command->actor.type == ActorConditionType::Name) {
        m_actor_selection_nameinput = command->name;
      }
      if (command->actor.type == ActorConditionType::Class) {
        m_actor_class = command->actor.checkId;
      }
      if (command->actor.type == ActorConditionType::Skill) {
        m_actor_skill = command->actor.checkId;
      }
      if (command->actor.type == ActorConditionType::Weapon) {
        m_actor_weapon = command->actor.checkId;
      }
      if (command->actor.type == ActorConditionType::Armor) {
        m_actor_armor = command->actor.checkId;
      }
      if (command->actor.type == ActorConditionType::State) {
        m_actor_state = command->actor.checkId;
      }
    }
    else if (command->type == ConditionType::Enemy) {
      m_enemy_selection = command->enemy.id;
      m_enemy_sub_selection = static_cast<int>(command->enemy.type);
      if (command->enemy.type == EnemyConditionType::State) {
        m_enemy_sub_state = command->enemy.stateId;
      }
      else {
        m_enemy_sub_state = 0;
      }
    }
    else if (command->type == ConditionType::Character) {
      m_character_selection = command->character.id;
      m_character_direction = static_cast<int>(command->character.facing);
    }
    else if (command->type == ConditionType::Vehicle) {
      m_vehicle_selection = static_cast<int>(command->vehicle.id);
    }
    else if (command->type == ConditionType::Gold) {
      m_gold_operation = static_cast<int>(command->gold.type);
      m_gold_selection = command->gold.value;
    }
    else if (command->type == ConditionType::Item) {
      m_item_selection = command->equip.equipId;
    }
    else if (command->type == ConditionType::Weapon) {
      m_weapon_selection = command->equip.equipId;
      m_weapon_include = command->equip.includeEquipment;
    }
    else if (command->type == ConditionType::Armor) {
      m_armor_selection = command->equip.equipId;
      m_armor_include = command->equip.includeEquipment;
    }
    else if (command->type == ConditionType::Button) {
     m_button_selection = static_cast<int>(command->button);
    }
    else if (command->type == ConditionType::Script) {
      m_script = command->script;
    }
    m_conditionType = static_cast<int>(command->type);
  }
  std::tuple<bool, bool> draw() override;

  Project* m_project = nullptr;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {
    std::shared_ptr<IEventCommand> sharedCommand = getCommand();
    eventCommands.push_back(sharedCommand);

    eventCommands.back()->indent = getParentIndent().value();
    eventCommands.push_back(std::make_shared<EventDummy>());
    eventCommands.back()->indent = getParentIndent().value() + 1;
    if (m_elseBranch) {
      eventCommands.push_back(std::make_shared<ElseCommand>());
      eventCommands.back()->indent = getParentIndent().value();
      eventCommands.push_back(std::make_shared<EventDummy>());
      eventCommands.back()->indent = getParentIndent().value() + 1;
    }
    eventCommands.push_back(std::make_shared<EndCommand>());
    eventCommands.back()->indent = getParentIndent().value();
    return eventCommands;
  };

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
  int m_enemy_selection{1};
  int m_enemy_sub_selection{0};
  int m_enemy_sub_state{1};
  int m_state_type{0};
  int m_character_selection{-1};
  int m_character_direction{1};
  int m_vehicle_selection{0};
  int m_gold_operation{0};
  int m_gold_selection{0};
  int m_item_selection{1};
  int m_weapon_selection{1};
  int m_weapon_type{0};
  bool m_weapon_include{false};
  int m_armor_selection{1};
  int m_armor_type{0};
  bool m_armor_include{false};
  int m_button_selection{0};
  std::string m_script;
  bool m_elseBranch{false};
  std::vector<std::shared_ptr<IEventCommand>> eventCommands;
  std::shared_ptr<ConditionalBranchCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ObjectPicker<State>> state_picker;
  std::optional<ObjectPicker<Weapon>> weapon_picker;
  std::optional<ObjectPicker<Armor>> armor_picker;
  std::optional<ObjectPicker<Skill>> skill_picker;
  std::optional<ObjectPicker<Class>> class_picker;
  std::optional<ObjectPicker<Item>> item_picker;
  bool m_confirmed{false};
  std::tuple<bool, bool> result;
};