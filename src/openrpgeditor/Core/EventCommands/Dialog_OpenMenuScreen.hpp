#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/OpenMenu.hpp"

struct Dialog_OpenMenuScreen : IEventDialogController {
  Dialog_OpenMenuScreen() = delete;
  explicit Dialog_OpenMenuScreen(const std::string& name,
                                 const std::shared_ptr<OpenMenuCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new OpenMenuCommand());
    }
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<OpenMenuCommand> command;
  std::tuple<bool, bool> result;
};