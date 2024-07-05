#pragma once
#include "IDialogController.hpp"
#include "Database/Actors.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/ControlTimer.hpp"

struct Project;
struct Dialog_TimerControl : IDialogController {
  Dialog_TimerControl() = delete;
  explicit Dialog_TimerControl(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ControlTimerCommand();
    m_operation = static_cast<int>(command->control);
    m_second = command->seconds;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:

  int m_operation;
  int m_minute = 0;
  int m_second;

  bool m_confirmed{false};
ControlTimerCommand* command;
  std::tuple<bool, bool> result;
};