#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/StopSE.hpp"

struct Dialog_StopSE : IEventDialogController {
  Dialog_StopSE() = delete;
  explicit Dialog_StopSE(const std::string& name,
                         const std::shared_ptr<StopSECommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new StopSECommand());
    }
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<StopSECommand> command;
  std::tuple<bool, bool> result;
};