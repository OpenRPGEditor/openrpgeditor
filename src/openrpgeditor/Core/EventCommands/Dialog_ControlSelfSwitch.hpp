#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ControlSelfSwitch.hpp"
#include "Database/States.hpp"

struct Dialog_ControlSelfSwitch : IEventDialogController {
  Dialog_ControlSelfSwitch() = delete;
  explicit Dialog_ControlSelfSwitch(const std::string& name, const std::shared_ptr<ControlSelfSwitchCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ControlSelfSwitchCommand());
    }
    command.reset(new ControlSelfSwitchCommand());
    m_selfSw = command->selfSw;
    m_turnOff = static_cast<int>(command->turnOff);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::string m_selfSw; // A, B, C, D
  int m_turnOff;

  bool m_confirmed{false};
  std::shared_ptr<ControlSelfSwitchCommand> command;
  std::tuple<bool, bool> result;
};