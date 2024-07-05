#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetWeatherEffectCommand final : IEventCommand {
  SetWeatherEffectCommand() = default;
  explicit SetWeatherEffectCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~SetWeatherEffectCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Weather_Effect; }
  WeatherEffect effect;
  int power;
  int duration;
  bool waitForCompletion;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
