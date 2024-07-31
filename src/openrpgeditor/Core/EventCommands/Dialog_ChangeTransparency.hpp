#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeTransparency.hpp"

struct Dialog_ChangeTransparency : IEventDialogController {
  Dialog_ChangeTransparency() = delete;
  explicit Dialog_ChangeTransparency(const std::string& name,
                                     const std::shared_ptr<ChangeTransparencyCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeTransparencyCommand());
    }
    isDisabled = static_cast<int>(command->transparency);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int isDisabled;
  bool m_confirmed{false};
  std::shared_ptr<ChangeTransparencyCommand> command;
  std::tuple<bool, bool> result;
};