#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeSaveAccess.hpp"

struct Dialog_ChangeSaveAccess : IEventDialogController {
  Dialog_ChangeSaveAccess() = delete;
  explicit Dialog_ChangeSaveAccess(const std::string& name, const std::shared_ptr<ChangeSaveAccessCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeSaveAccessCommand());
    }
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeSaveAccessCommand> command;
  std::tuple<bool, bool> result;
};