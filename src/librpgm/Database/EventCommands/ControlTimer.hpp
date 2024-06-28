#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlTimer : IEventCommand {
  ~ControlTimer() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Self_Switch; }
  TimerControl control;
  int seconds;
  [[nodiscard]] std::string stringRep() const override {
    std::string min;
    std::string sec;

    if (seconds > 59) {
      min = std::to_string(seconds / 60);
      sec = std::to_string(seconds % 60);
    } else {
      min = "0";
      sec = std::to_string(seconds);
    }
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Control Timer : " + DecodeEnumName(control) + ", " + min + " min " +
           sec + " sec" + ColorFormatter::popColor();
  }
};
