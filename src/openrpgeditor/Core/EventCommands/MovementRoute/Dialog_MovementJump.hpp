#pragma once
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/MovementRoute/Jump.hpp"

struct MainWindow;
struct Dialog_MovementJump : IEventDialogController {
  Dialog_MovementJump() = delete;
  explicit Dialog_MovementJump(const std::string& name, const std::shared_ptr<MovementJumpCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (m_command == nullptr) {
      m_command.reset(new MovementJumpCommand());
    }
    m_jumpX = m_command->x;
    m_jumpY = m_command->y;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_jumpX;
  int m_jumpY;
  std::shared_ptr<MovementJumpCommand> m_command;
};