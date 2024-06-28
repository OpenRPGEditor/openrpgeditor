#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EndCommand : IEventCommand {
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
  [[nodiscard]] std::string stringRep(const Database& db) const override { return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "End"; }
};
