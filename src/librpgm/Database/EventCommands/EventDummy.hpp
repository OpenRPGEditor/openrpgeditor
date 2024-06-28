#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/ColorFormatter.hpp"

struct EventDummy : IEventCommand {
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + ColorFormatter::getColorCode(code()) + symbol(code()) + ColorFormatter::popColor();
  }
};