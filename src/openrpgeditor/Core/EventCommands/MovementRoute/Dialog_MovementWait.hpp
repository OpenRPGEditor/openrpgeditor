#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/MovementRoute/Wait.hpp"

struct Project;
struct Dialog_MovementWait : IEventDialogController {
  Dialog_MovementWait() = delete;
  explicit Dialog_MovementWait(const std::string& name) : IEventDialogController(name) {
    command.reset(new MovementWaitCommand());
    m_waitDuration = command->duration;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_waitDuration;
  bool m_confirmed{false};
  std::shared_ptr<MovementWaitCommand> command;
  std::tuple<bool, bool> result;
};