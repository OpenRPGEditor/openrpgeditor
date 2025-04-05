#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/BreakLoop.hpp"

struct Dialog_BreakLoop : IEventDialogController {
  Dialog_BreakLoop() = delete;
  explicit Dialog_BreakLoop(const std::string& name, const std::shared_ptr<BreakLoopCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new BreakLoopCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<BreakLoopCommand> command;
  std::tuple<bool, bool> result;
};