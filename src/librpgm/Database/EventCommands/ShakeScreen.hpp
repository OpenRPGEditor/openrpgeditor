#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShakeScreenCommand : IEventCommand {
  ~ShakeScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shake_Screen; }
  int power;
  int speed;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Shake Screen : " + std::format("{}, {}, {} frames", power, speed, duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};
