#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct InputNumberCommand : IEventCommand {
  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
  int variable;
  int digits;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Select Item : {}, " + (digits > 1 ? " digits" : " digit") + ColorFormatter::popColor();
  }
};
