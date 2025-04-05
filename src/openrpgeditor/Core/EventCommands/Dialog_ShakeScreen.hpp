#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ShakeScreen.hpp"

struct Dialog_ShakeScreen : IEventDialogController {
  Dialog_ShakeScreen() = delete;
  explicit Dialog_ShakeScreen(const std::string& name, const std::shared_ptr<ShakeScreenCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ShakeScreenCommand());
    }
    m_power = command->power;
    m_speed = command->speed;
    m_duration = command->duration;
    m_waitForCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_power;
  int m_speed;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<ShakeScreenCommand> command;
  std::tuple<bool, bool> result;
};