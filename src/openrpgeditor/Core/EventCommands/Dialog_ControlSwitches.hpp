#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/ControlSwitches.hpp"

struct Project;
struct Dialog_ControlSwitches : IDialogController {
  Dialog_ControlSwitches() = delete;
  explicit Dialog_ControlSwitches(const std::string& name, Project* project) : IDialogController(name), m_project(project) {}
  std::tuple<bool, bool>  draw() override;

  Project* m_project = nullptr;

  std::shared_ptr<IEventCommand> getCommand() override {
    return std::make_shared<ControlSwitches>(command.value());
  };

private:
  int d_start = 1;
  int d_end = 1;
  int d_rand_1 = 0;
  int d_rand_2 = 0;

  std::optional<ControlSwitches> command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};