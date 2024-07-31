#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/MovementRoute/Jump.hpp"

struct Project;
struct Dialog_MovementJump : IEventDialogController {
  Dialog_MovementJump() = delete;
  explicit Dialog_MovementJump(const std::string& name) : IEventDialogController(name) {
    command.reset(new MovementJumpCommand());
    m_jump_x = command->x;
    m_jump_y = command->y;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_jump_x;
  int m_jump_y;
  bool m_confirmed{false};
  std::shared_ptr<MovementJumpCommand> command;
  std::tuple<bool, bool> result;
};