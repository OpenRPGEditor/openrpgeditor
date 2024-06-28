#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlSelfSwitch : IEventCommand {
  ~ControlSelfSwitch() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }

  std::string selfSw; // A, B, C, D
  ValueControl turnOff;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Self Switch : " + selfSw + " is " + DecodeEnumName(turnOff) + ColorFormatter::popColor();
  }
};