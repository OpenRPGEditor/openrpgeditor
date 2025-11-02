#pragma once
#include "Database/EventCommands/MovementRoute/BlendMode.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_MovementChangeBlendMode : IEventDialogController {
  Dialog_MovementChangeBlendMode() = delete;
  explicit Dialog_MovementChangeBlendMode(const std::string& name, const std::shared_ptr<MovementChangeBlendModeCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (command == nullptr) {
      command.reset(new MovementChangeBlendModeCommand());
    }
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