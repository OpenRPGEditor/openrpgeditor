#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Project.hpp"
#include "Database/EventCommands/MovementRoute/SwitchOFF.hpp"

struct Project;
struct Dialog_MovementSwitchOFF : IEventDialogController {
  Dialog_MovementSwitchOFF() = delete;
  explicit Dialog_MovementSwitchOFF(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new MovementSwitchOFFCommand());
    picker.emplace("Switches", m_project->system().switches);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

  Project* m_project = nullptr;

private:
  bool m_confirmed{false};
  std::shared_ptr<MovementSwitchOFFCommand> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};