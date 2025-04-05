#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/OpenSave.hpp"

struct Dialog_OpenSaveScreen : IEventDialogController {
  Dialog_OpenSaveScreen() = delete;
  explicit Dialog_OpenSaveScreen(const std::string& name, const std::shared_ptr<OpenSaveCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new OpenSaveCommand());
    }
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<OpenSaveCommand> command;
  std::tuple<bool, bool> result;
};