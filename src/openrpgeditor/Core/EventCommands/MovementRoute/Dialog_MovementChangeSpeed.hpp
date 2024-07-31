#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/MovementRoute/Speed.hpp"
struct Project;
struct Dialog_MovementChangeSpeed : IEventDialogController {
  Dialog_MovementChangeSpeed() = delete;
  explicit Dialog_MovementChangeSpeed(const std::string& name) : IEventDialogController(name) {
    command.reset(new MovementSpeedCommand());
    m_speed = command->speed;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_speed;
  bool m_confirmed{false};
  std::shared_ptr<MovementSpeedCommand> command;
  std::tuple<bool, bool> result;
};