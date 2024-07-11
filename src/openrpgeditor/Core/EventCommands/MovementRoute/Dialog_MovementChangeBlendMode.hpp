#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/MovementRoute/BlendMode.hpp"

struct Project;
struct Dialog_MovementChangeBlendMode : IEventDialogController {
  Dialog_MovementChangeBlendMode() = delete;
  explicit Dialog_MovementChangeBlendMode(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new MovementChangeBlendModeCommand());
    m_blendMode = static_cast<int>(command->mode);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_blendMode;
  bool m_confirmed{false};
  std::shared_ptr<MovementChangeBlendModeCommand> command;
  std::tuple<bool, bool> result;
};