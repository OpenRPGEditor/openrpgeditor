#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ExitEventProecessingCommand : IEventCommand {
  ~ExitEventProecessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Exit_Event_Processing; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Exit Event Processing";
  }
};