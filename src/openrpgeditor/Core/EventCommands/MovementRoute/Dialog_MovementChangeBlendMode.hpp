#pragma once
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/MovementRoute/BlendMode.hpp"

struct Dialog_MovementChangeBlendMode : IEventDialogController {
  Dialog_MovementChangeBlendMode() = delete;
  explicit Dialog_MovementChangeBlendMode(const std::string& name) : IEventDialogController(name) {
    command.reset(new MovementChangeBlendModeCommand());
    m_blendMode = static_cast<int>(command->mode);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_blendMode;
  bool m_confirmed{false};
  std::shared_ptr<MovementChangeBlendModeCommand> command;
  std::tuple<bool, bool> result;
};