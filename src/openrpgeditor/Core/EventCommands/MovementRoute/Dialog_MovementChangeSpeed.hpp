#pragma once

#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/MovementRoute/Speed.hpp"

struct Dialog_MovementChangeSpeed final : IEventDialogController {
  Dialog_MovementChangeSpeed() = delete;
  explicit Dialog_MovementChangeSpeed(const std::string& name, const std::shared_ptr<MovementSpeedCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (m_command == nullptr) {
      m_command.reset(new MovementSpeedCommand());
    }
    m_speed = m_command->speed;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_speed;
  std::shared_ptr<MovementSpeedCommand> m_command;
};