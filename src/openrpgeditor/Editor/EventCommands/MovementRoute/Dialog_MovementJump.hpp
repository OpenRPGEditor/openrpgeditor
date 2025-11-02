#pragma once
#include "Database/EventCommands/MovementRoute/Jump.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

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