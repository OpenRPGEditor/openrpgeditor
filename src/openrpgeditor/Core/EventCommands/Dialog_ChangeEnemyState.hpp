#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/States.hpp"
#include "Database/EventCommands/ChangeEnemyState.hpp"

struct Project;
struct Dialog_ChangeEnemyState : IDialogController {
  Dialog_ChangeEnemyState() = delete;
  explicit Dialog_ChangeEnemyState(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeEnemyStateCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int m_troop_selection = 0;
  int m_operation = 0; // Party Member Operation
  int m_state = 1;

  bool m_confirmed{false};
  std::optional<ObjectPicker<State>> state_picker;
  std::optional<ChangeEnemyStateCommand> command;
  std::tuple<bool, bool> result;
};