#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/FadeinScreen.hpp"

struct Dialog_FadeinScreen : IEventDialogController {
  Dialog_FadeinScreen() = delete;
  explicit Dialog_FadeinScreen(const std::string& name, const std::shared_ptr<FadeinScreenCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new FadeinScreenCommand());
    }
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<FadeinScreenCommand> command;
  std::tuple<bool, bool> result;
};