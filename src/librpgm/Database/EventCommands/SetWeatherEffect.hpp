#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetWeatherEffectCommand : IEventCommand {
  ~SetWeatherEffectCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Weather_Effect; }
  WeatherEffect effect;
  int power;
  int duration;
  bool waitForCompletion;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Weather Effect : " + DecodeEnumName(effect) + ", " + std::to_string(duration) + " + frames"
    + (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};
