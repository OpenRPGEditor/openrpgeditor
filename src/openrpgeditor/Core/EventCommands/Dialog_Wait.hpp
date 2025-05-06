#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeNickname.hpp"
#include "Database/EventCommands/Wait.hpp"

struct Dialog_Wait : IEventDialogController {
  Dialog_Wait() = delete;
  explicit Dialog_Wait(const std::string& name, const std::shared_ptr<WaitCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new WaitCommand());
    }
    m_waitDuration = m_command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_waitDuration;
  bool m_confirmed{false};
  std::shared_ptr<WaitCommand> m_command;
};