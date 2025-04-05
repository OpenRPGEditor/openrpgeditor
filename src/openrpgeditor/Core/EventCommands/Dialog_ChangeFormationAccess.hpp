#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeFormationAccess.hpp"

struct Dialog_ChangeFormationAccess : IEventDialogController {
  Dialog_ChangeFormationAccess() = delete;
  explicit Dialog_ChangeFormationAccess(const std::string& name, const std::shared_ptr<ChangeFormationAccessCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeFormationAccessCommand());
    }
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeFormationAccessCommand> command;
  std::tuple<bool, bool> result;
};