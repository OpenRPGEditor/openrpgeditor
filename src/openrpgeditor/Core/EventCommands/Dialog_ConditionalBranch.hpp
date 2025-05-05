#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Actors.hpp"
#include "Database/Armors.hpp"
#include "Database/Classes.hpp"
#include "Database/EventCommands/ConditionalBranch.hpp"
#include "Database/EventCommands/End.hpp"
#include "Database/EventCommands/EventDummy.hpp"
#include "Database/Items.hpp"
#include "Database/Skills.hpp"
#include "Database/States.hpp"
#include "Database/Weapons.hpp"

struct Dialog_ConditionalBranch : IEventDialogController {
  Dialog_ConditionalBranch() = delete;
  explicit Dialog_ConditionalBranch(const std::string& name, const std::shared_ptr<ConditionalBranchCommand>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ConditionalBranchCommand());
    }

    if (m_command->type == ConditionType::Switch) {
      m_switchId = m_command->globalSwitch.switchIdx;
      m_switchValue = static_cast<int>(m_command->globalSwitch.checkIfOn);
    } else if (m_command->type == ConditionType::Variable) {
      m_variableId = m_command->variable.id;
      m_variableSubSource = static_cast<int>(m_command->variable.source);
      m_variableValue = static_cast<int>(m_command->variable.comparison);
      if (m_command->variable.source == VariableComparisonSource::Constant) {
        m_subConstant = m_command->variable.constant;
      } else {
        m_subVariableId = m_command->variable.otherId;
      }
    } else if (m_command->type == ConditionType::Self_Switch) {
      m_selfSw = m_command->selfSw;
      m_selfSwValue = static_cast<int>(m_command->selfSwitch.checkIfOn);
    } else if (m_command->type == ConditionType::Timer) {
      m_timerMin = m_command->timer.sec / 60;
      m_timerSec = m_command->timer.sec % 60;
      m_timerOperation = static_cast<int>(m_command->timer.comparison);
    } else if (m_command->type == ConditionType::Actor) {
      m_actorSelection = m_command->actor.id;
      m_actorSubSelection = static_cast<int>(m_command->actor.type);
      if (m_command->actor.type == ActorConditionType::In_The_Party) {
        //
      }
      if (m_command->actor.type == ActorConditionType::Name) {
        m_actorSelectionNameInput = m_command->name;
      }
      if (m_command->actor.type == ActorConditionType::Class) {
        m_actorClass = m_command->actor.checkId;
      }
      if (m_command->actor.type == ActorConditionType::Skill) {
        m_actorSkill = m_command->actor.checkId;
      }
      if (m_command->actor.type == ActorConditionType::Weapon) {
        m_actorWeapon = m_command->actor.checkId;
      }
      if (m_command->actor.type == ActorConditionType::Armor) {
        m_actorArmor = m_command->actor.checkId;
      }
      if (m_command->actor.type == ActorConditionType::State) {
        m_actorState = m_command->actor.checkId;
      }
    } else if (m_command->type == ConditionType::Enemy) {
      m_enemySelection = m_command->enemy.id;
      m_enemySubSelection = static_cast<int>(m_command->enemy.type);
      if (m_command->enemy.type == EnemyConditionType::State) {
        m_enemySubState = m_command->enemy.stateId;
      } else {
        m_enemySubState = 0;
      }
    } else if (m_command->type == ConditionType::Character) {
      m_characterSelection = m_command->character.id;
      m_characterDirection = static_cast<int>(m_command->character.facing);
    } else if (m_command->type == ConditionType::Vehicle) {
      m_vehicleSelection = static_cast<int>(m_command->vehicle.id);
    } else if (m_command->type == ConditionType::Gold) {
      m_goldOperation = static_cast<int>(m_command->gold.type);
      m_goldSelection = m_command->gold.value;
    } else if (m_command->type == ConditionType::Item) {
      m_itemSelection = m_command->equip.equipId;
    } else if (m_command->type == ConditionType::Weapon) {
      m_weaponSelection = m_command->equip.equipId;
      m_weaponInclude = m_command->equip.includeEquipment;
    } else if (m_command->type == ConditionType::Armor) {
      m_armorSelection = m_command->equip.equipId;
      m_armorInclude = m_command->equip.includeEquipment;
    } else if (m_command->type == ConditionType::Button) {
      m_buttonSelection = static_cast<int>(m_command->button);
    } else if (m_command->type == ConditionType::Script) {
      m_script.SetTextLines(splitString(m_command->script, '\n'));
    }
    m_conditionType = static_cast<int>(m_command->type);

    if (m_conditionType == 4) {
      m_selectedTab = 1;
    }
    if (m_conditionType == 5 || m_conditionType == 6 || m_conditionType == 13) {
      m_selectedTab = 2;
    }
    if (m_conditionType < 13 && m_conditionType > 6) {
      m_selectedTab = 3;
    }

    // if (command->isPartner(EventCode::Else, command->indent())) {
    //   m_else = true;
    //   m_elseBranch = true;
    // }

    m_script.SetLanguageDefinition(TextEditor::LanguageDefinition::Javascript());
  }
  void drawPickers();
  void drawMiscTab();
  void drawEntityTab();
  void drawActorTab();
  void drawDataTab();
  void drawDialogButtons();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };
  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands(std::vector<std::shared_ptr<IEventCommand>>& list) override {
    std::vector<std::shared_ptr<IEventCommand>> eventCommands;
    std::shared_ptr<IEventCommand> sharedCommand = getCommand();
    eventCommands.push_back(sharedCommand);

    eventCommands.back()->setIndent(getParentIndent().value());
    eventCommands.insert(eventCommands.end(), list.begin(), list.end());
    if (m_elseBranch) {
      eventCommands.push_back(std::make_shared<ElseCommand>());
      eventCommands.back()->setIndent(getParentIndent().value());
      eventCommands.push_back(std::make_shared<EventDummy>());
      eventCommands.back()->setIndent(getParentIndent().value() + 1);
    }
    eventCommands.push_back(std::make_shared<EndCommand>());
    eventCommands.back()->setIndent(getParentIndent().value());
    return eventCommands;
  };
  std::vector<std::shared_ptr<IEventCommand>> getBatchCommands() override {
    std::vector<std::shared_ptr<IEventCommand>> eventCommands;
    const std::shared_ptr<IEventCommand> sharedCommand = getCommand();
    eventCommands.push_back(sharedCommand);

    eventCommands.back()->setIndent(getParentIndent().value());
    eventCommands.push_back(std::make_shared<EventDummy>());
    eventCommands.back()->setIndent(getParentIndent().value() + 1);
    if (m_elseBranch) {
      eventCommands.push_back(std::make_shared<ElseCommand>());
      eventCommands.back()->setIndent(getParentIndent().value());
      eventCommands.push_back(std::make_shared<EventDummy>());
      eventCommands.back()->setIndent(getParentIndent().value() + 1);
    }
    eventCommands.push_back(std::make_shared<EndCommand>());
    eventCommands.back()->setIndent(getParentIndent().value());
    return eventCommands;
  };
  bool isCurrentElseBranch() override { return m_else == m_elseBranch; }
  void setCurrentElseBranch() override { m_else = m_elseBranch; }
  void setElse(const bool cond) override {
    m_else = cond;
    m_elseBranch = cond;
  }

private:
  int m_troopId = 0;
  int m_selectedTab{0};
  int m_conditionType{0};
  int m_switchId{1};
  int m_switchValue{0};
  int m_pickerType;
  int m_variableId{1};
  int m_variableValue{0};
  int m_variableSubSource{0};
  int m_subConstant{0};
  int m_subVariableId{1};
  std::string m_selfSw{"A"};
  int m_selfSwValue{0};
  int m_timerMin{0};
  int m_timerSec{0};
  int m_timerOperation{0};
  int m_actorSelection{0};
  int m_actorSubSelection{0};
  std::string m_actorSelectionNameInput;
  int m_actorClass{1};
  int m_actorSkill{1};
  int m_actorArmor{1};
  int m_actorWeapon{1};
  int m_actorState{1};
  int m_enemySelection{0};
  int m_enemySubSelection{0};
  int m_enemySubState{1};
  int m_stateType{0};
  int m_characterSelection{-1};
  int m_characterDirection{1};
  int m_vehicleSelection{0};
  int m_goldOperation{0};
  int m_goldSelection{0};
  int m_itemSelection{1};
  int m_weaponSelection{1};
  int m_weaponType{0};
  bool m_weaponInclude{false};
  int m_armorSelection{1};
  int m_armorType{0};
  bool m_armorInclude{false};
  int m_buttonSelection{0};
  TextEditor m_script;
  bool m_elseBranch{false}; // determines future else (the option)
  bool m_else{false};       // determines current else
  std::shared_ptr<ConditionalBranchCommand> m_command;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<State>> m_statePicker;
  std::optional<ObjectPicker<Weapon>> m_weaponPicker;
  std::optional<ObjectPicker<Armor>> m_armorPicker;
  std::optional<ObjectPicker<Skill>> m_skillPicker;
  std::optional<ObjectPicker<Class>> m_classPicker;
  std::optional<ObjectPicker<Item>> m_itemPicker;
  bool m_confirmed{false};
};