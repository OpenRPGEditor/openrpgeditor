#pragma once
#include "Database/EventCommands/AbortBattle.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_AbortBattle : IEventDialogController {
  Dialog_AbortBattle() = delete;
  explicit Dialog_AbortBattle(const std::string& name, const std::shared_ptr<AbortBattleCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new AbortBattleCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<AbortBattleCommand> command;
  std::tuple<bool, bool> result;
};