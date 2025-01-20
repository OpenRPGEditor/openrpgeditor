#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/SaveBGM.hpp"

struct Dialog_SaveBGM : IEventDialogController {
  Dialog_SaveBGM() = delete;
  explicit Dialog_SaveBGM(const std::string& name, const std::shared_ptr<SaveBGMCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new SaveBGMCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<SaveBGMCommand> command;
  std::tuple<bool, bool> result;
};