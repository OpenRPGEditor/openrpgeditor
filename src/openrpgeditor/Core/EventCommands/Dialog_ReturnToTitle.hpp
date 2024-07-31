#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ReturnToTitle.hpp"

struct Dialog_ReturnToTitle : IEventDialogController {
  Dialog_ReturnToTitle() = delete;
  explicit Dialog_ReturnToTitle(const std::string& name,
                                const std::shared_ptr<ReturnToTitleCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ReturnToTitleCommand());
    }
    m_open = true;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  std::shared_ptr<ReturnToTitleCommand> command;
  std::tuple<bool, bool> result;
};