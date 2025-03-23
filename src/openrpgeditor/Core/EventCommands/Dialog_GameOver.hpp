#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/GameOver.hpp"

struct Dialog_GameOver : IEventDialogController {
  Dialog_GameOver() = delete;
  explicit Dialog_GameOver(const std::string& name, const std::shared_ptr<GameOverCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new GameOverCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;
  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<GameOverCommand> command;
  std::tuple<bool, bool> result;
};