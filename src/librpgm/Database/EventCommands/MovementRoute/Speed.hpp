#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovementSpeedCommand : IEventCommand {
  ~MovementSpeedCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Speed; }
  int speed;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Speed : " + std::to_string(speed) + ColorFormatter::popColor();
  }
};
