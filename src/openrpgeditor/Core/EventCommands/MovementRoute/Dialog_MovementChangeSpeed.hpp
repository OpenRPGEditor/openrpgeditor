#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/TextEditor.hpp"
#include "Database/EventCommands/MovementRoute/Speed.hpp"
struct Project;
struct Dialog_MovementChangeSpeed : IEventDialogController {
  Dialog_MovementChangeSpeed() = delete;
  explicit Dialog_MovementChangeSpeed(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new MovementSpeedCommand());
    m_speed = command->speed;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_speed;
  bool m_confirmed{false};
  std::shared_ptr<MovementSpeedCommand> command;
  std::tuple<bool, bool> result;
};