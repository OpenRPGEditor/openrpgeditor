#pragma once
#include "IDialogController.hpp"
#include "Database/States.hpp"
#include "Database/EventCommands/ControlSelfSwitch.hpp"

struct Project;
struct Dialog_ControlSelfSwitch : IDialogController {
  Dialog_ControlSelfSwitch() = delete;
  explicit Dialog_ControlSelfSwitch(const std::string& name, Project* project)
  : IDialogController(name), m_project(project) {
    command.reset(new ControlSelfSwitchCommand());
    m_selfSw = command->selfSw;
    m_turnOff = static_cast<int>(command->turnOff);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  std::string m_selfSw; // A, B, C, D
  int m_turnOff;

  bool m_confirmed{false};
  std::shared_ptr<ControlSelfSwitchCommand> command;
  std::tuple<bool, bool> result;
};