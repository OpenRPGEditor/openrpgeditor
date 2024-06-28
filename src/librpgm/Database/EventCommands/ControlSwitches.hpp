#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlSwitches : IEventCommand {
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  int start;
  int end;
  ValueControl turnOff;

  [[nodiscard]] std::string stringRep() const override {
    if (start != end) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Switches : #" + std::format("{:04}", start) + ".." + std::format("{:04}", end) + " = " +
             (turnOff == ValueControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
    } else {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Switches : #" + std::format("{:04}", start) + " {}" + " = " +
             (turnOff == ValueControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
    }
  }
};
