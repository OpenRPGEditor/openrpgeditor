#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/ControlSwitches.hpp"

struct Project;
struct Dialog_ControlSwitches : IEventDialogController {
  Dialog_ControlSwitches() = delete;
  explicit Dialog_ControlSwitches(const std::string& name, Project* project)
  : IEventDialogController(name), m_project(project) {
    command.reset(new ControlSwitches());
    m_start = command->start;
    m_end = command->end;
    m_isDisabled = static_cast<int>(command->turnOff);
  }
  std::tuple<bool, bool> draw() override;

  Project* m_project = nullptr;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_start;
  int m_end;
  int m_rand_1{1};
  int m_rand_2{1};
  int m_isDisabled;
  int m_operation{0};

  std::shared_ptr<ControlSwitches> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};