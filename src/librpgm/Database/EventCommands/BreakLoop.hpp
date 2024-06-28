#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct BreakLoopCommand : IEventCommand {
  ~BreakLoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Break_Loop; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Break Loop";
  }
};