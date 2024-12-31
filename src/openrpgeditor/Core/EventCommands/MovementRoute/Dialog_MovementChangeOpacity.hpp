#pragma once
#include "Core/CommonUI/TextEditor.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"
#include "Database/EventCommands/MovementRoute/Wait.hpp"

struct MainWindow;
struct Dialog_MovementChangeOpacity : IEventDialogController {
  Dialog_MovementChangeOpacity() = delete;
  explicit Dialog_MovementChangeOpacity(const std::string& name) : IEventDialogController(name) {
    command.reset(new MovementChangeOpacityCommand());
    m_opacity = command->opacity;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_opacity;
  bool m_confirmed{false};
  std::shared_ptr<MovementChangeOpacityCommand> command;
  std::tuple<bool, bool> result;
};