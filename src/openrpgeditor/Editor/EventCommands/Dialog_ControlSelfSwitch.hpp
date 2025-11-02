#pragma once
#include "Database/EventCommands/ControlSelfSwitch.hpp"
#include "Database/States.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ControlSelfSwitch : IEventDialogController {
  Dialog_ControlSelfSwitch() = delete;
  explicit Dialog_ControlSelfSwitch(const std::string& name, const std::shared_ptr<ControlSelfSwitchCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ControlSelfSwitchCommand());
    }
    m_command.reset(new ControlSelfSwitchCommand());
    m_selfSw = m_command->selfSw;
    m_turnOff = static_cast<int>(m_command->turnOff);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  std::string m_selfSw; // A, B, C, D
  int m_turnOff;

  bool m_confirmed{false};
  std::shared_ptr<ControlSelfSwitchCommand> m_command;
};