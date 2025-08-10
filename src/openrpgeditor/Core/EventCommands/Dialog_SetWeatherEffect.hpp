#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/SetWeatherEffect.hpp"

struct Dialog_SetWeatherEffect : IEventDialogController {
  Dialog_SetWeatherEffect() = delete;
  explicit Dialog_SetWeatherEffect(const std::string& name, const std::shared_ptr<SetWeatherEffectCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new SetWeatherEffectCommand());
    }
    m_effect = static_cast<int>(m_command->effect);
    m_power = m_command->power;
    m_duration = m_command->duration;
    m_waitForCompletion = m_command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_effect;
  int m_power;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<SetWeatherEffectCommand> m_command;
};