#pragma once
#include "Database/EventCommands/OpenMenu.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_OpenMenuScreen : IEventDialogController {
  Dialog_OpenMenuScreen() = delete;
  explicit Dialog_OpenMenuScreen(const std::string& name, const std::shared_ptr<OpenMenuCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new OpenMenuCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<OpenMenuCommand> command;
  std::tuple<bool, bool> result;
};