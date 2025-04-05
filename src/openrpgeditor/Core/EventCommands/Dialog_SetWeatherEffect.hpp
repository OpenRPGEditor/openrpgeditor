#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/SetWeatherEffect.hpp"

struct Dialog_SetWeatherEffect : IEventDialogController {
  Dialog_SetWeatherEffect() = delete;
  explicit Dialog_SetWeatherEffect(const std::string& name, const std::shared_ptr<SetWeatherEffectCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new SetWeatherEffectCommand());
    }
    m_effect = static_cast<int>(command->effect);
    m_power = command->power;
    m_duration = command->duration;
    m_waitForCompletion = command->waitForCompletion;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_effect;
  int m_power;
  int m_duration;
  bool m_waitForCompletion;

  bool m_confirmed{false};
  std::shared_ptr<SetWeatherEffectCommand> command;
  std::tuple<bool, bool> result;
};