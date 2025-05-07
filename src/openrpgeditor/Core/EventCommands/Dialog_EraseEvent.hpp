#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/EraseEvent.hpp"

struct Dialog_EraseEvent : IEventDialogController {
  Dialog_EraseEvent() = delete;
  explicit Dialog_EraseEvent(const std::string& name, const std::shared_ptr<EraseEventCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new EraseEventCommand());
    }
    m_open = false;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  bool m_confirmed{true};
  std::shared_ptr<EraseEventCommand> m_command;
};