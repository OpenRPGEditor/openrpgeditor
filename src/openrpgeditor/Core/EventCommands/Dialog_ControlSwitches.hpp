#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Database/EventCommands/ControlSwitches.hpp"

struct Project;
struct Dialog_ControlSwitches : IDialogController {
  Dialog_ControlSwitches() = delete;
  explicit Dialog_ControlSwitches(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
    command = new ControlSwitches();
    m_start = command->start;
    m_end = command->end;
    isDisabled = static_cast<int>(command->turnOff);
  }
  std::tuple<bool, bool>  draw() override;

  Project* m_project = nullptr;

  IEventCommand* getCommand() override {
    return command;
  };

private:
  int m_start;
  int m_end;
  int m_rand_1 = 0;
  int m_rand_2 = 0;
  int isDisabled;

  ControlSwitches* command;
  std::optional<VariableSwitchPicker> picker;
  std::tuple<bool, bool> result;
};