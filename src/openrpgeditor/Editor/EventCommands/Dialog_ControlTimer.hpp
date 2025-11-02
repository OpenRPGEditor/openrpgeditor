#pragma once
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/ControlTimer.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ControlTimer : IEventDialogController {
  Dialog_ControlTimer() = delete;
  explicit Dialog_ControlTimer(const std::string& name, const std::shared_ptr<ControlTimerCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ControlTimerCommand());
    }
    m_operation = static_cast<int>(m_command->control);
    m_minute = m_command->seconds / 60;
    m_second = m_command->seconds % 60;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_operation;
  int m_minute = 0;
  int m_second;

  bool m_confirmed{false};
  std::shared_ptr<ControlTimerCommand> m_command;
};