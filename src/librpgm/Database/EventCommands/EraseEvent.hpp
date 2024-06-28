#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EraseEventCommand : IEventCommand {
  ~EraseEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Event; }[[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Erase Event" + ColorFormatter::popColor();
  }
};
