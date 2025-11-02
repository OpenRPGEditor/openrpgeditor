#pragma once
#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"
#include "Database/EventCommands/MovementRoute/Wait.hpp"
#include "Editor/CommonUI/TextEditor.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct MainWindow;
struct Dialog_MovementChangeOpacity : IEventDialogController {
  Dialog_MovementChangeOpacity() = delete;
  explicit Dialog_MovementChangeOpacity(const std::string& name, const std::shared_ptr<MovementChangeOpacityCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (command == nullptr) {
      command.reset(new MovementChangeOpacityCommand());
    }
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