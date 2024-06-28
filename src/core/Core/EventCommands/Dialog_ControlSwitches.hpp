#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/ControlSwitches.hpp"

struct Project;
struct Dialog_ControlSwitches : IDialogController {
  Dialog_ControlSwitches() = delete;
  explicit Dialog_ControlSwitches(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command->start = 0;
    command->end = 0;
  }
  std::tuple<bool, bool>  draw() override;

  Project* m_project = nullptr;
private:
  std::optional<ControlSwitches> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};