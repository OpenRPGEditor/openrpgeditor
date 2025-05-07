#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ExitEventProcessing.hpp"

struct Dialog_ExitEventProcessing : IEventDialogController {
  Dialog_ExitEventProcessing() = delete;
  explicit Dialog_ExitEventProcessing(const std::string& name, const std::shared_ptr<ExitEventProcessingCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ExitEventProcessingCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<ExitEventProcessingCommand> m_command;
};