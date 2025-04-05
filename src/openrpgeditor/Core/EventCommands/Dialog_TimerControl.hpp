#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/ControlTimer.hpp"

struct Dialog_TimerControl : IEventDialogController {
  Dialog_TimerControl() = delete;
  explicit Dialog_TimerControl(const std::string& name, const std::shared_ptr<ControlTimerCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ControlTimerCommand());
    }
    m_operation = static_cast<int>(command->control);
    m_second = command->seconds;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_operation;
  int m_minute = 0;
  int m_second;

  bool m_confirmed{false};
  std::shared_ptr<ControlTimerCommand> command;
  std::tuple<bool, bool> result;
};