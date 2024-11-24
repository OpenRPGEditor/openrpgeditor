#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeMenuAccess.hpp"

struct Dialog_ChangeMenuAccess : IEventDialogController {
  Dialog_ChangeMenuAccess() = delete;
  explicit Dialog_ChangeMenuAccess(const std::string& name, const std::shared_ptr<ChangeMenuAccessCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeMenuAccessCommand());
    }
    isDisabled = static_cast<int>(command->access);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeMenuAccessCommand> command;
  std::tuple<bool, bool> result;
};