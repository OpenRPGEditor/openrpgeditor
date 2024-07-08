#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/States.hpp"
#include "Database/EventCommands/ChangeEnemyState.hpp"

struct Project;
struct Dialog_ChangeEnemyState : IEventDialogController {
  Dialog_ChangeEnemyState() = delete;
  explicit Dialog_ChangeEnemyState(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ChangeEnemyStateCommand());
    m_troop_selection = command->enemy;
    m_operation = static_cast<int>(command->enemyOp);
    m_state = command->state;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_troop_selection;
  int m_operation; // Party Member Operation
  int m_state;

  bool m_confirmed{false};
  std::optional<ObjectPicker<State>> state_picker;
  std::shared_ptr<ChangeEnemyStateCommand> command;
  std::tuple<bool, bool> result;
};