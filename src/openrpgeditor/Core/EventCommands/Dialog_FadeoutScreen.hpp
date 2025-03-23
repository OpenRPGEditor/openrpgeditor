#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/FadeoutScreen.hpp"

struct Dialog_FadeoutScreen : IEventDialogController {
  Dialog_FadeoutScreen() = delete;
  explicit Dialog_FadeoutScreen(const std::string& name, const std::shared_ptr<FadeoutScreenCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new FadeoutScreenCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<FadeoutScreenCommand> command;
  std::tuple<bool, bool> result;
};