#pragma once
#include "Database/EventCommands/MovementRoute/Wait.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct MainWindow;
struct Dialog_MovementWait : IEventDialogController {
  Dialog_MovementWait() = delete;
  explicit Dialog_MovementWait(const std::string& name, const std::shared_ptr<MovementWaitCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (command == nullptr) {
      command.reset(new MovementWaitCommand());
    }
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