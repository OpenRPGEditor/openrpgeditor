#pragma once
#include "Database/EventCommands/ShakeScreen.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ShakeScreen : IEventDialogController {
  Dialog_ShakeScreen() = delete;
  explicit Dialog_ShakeScreen(const std::string& name, const std::shared_ptr<ShakeScreenCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ShakeScreenCommand());
    }
    m_power = m_command->power;
    m_speed = m_command->speed;
    m_duration = m_command->duration;
    m_waitForCompletion = m_command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_power;
  int m_speed;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<ShakeScreenCommand> m_command;
  std::tuple<bool, bool> result;
};