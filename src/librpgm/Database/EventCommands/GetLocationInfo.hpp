#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GetLocationInfoCommand : IEventCommand {
  ~GetLocationInfoCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_Location_Info; }
  int variable;
  TileType type;
  LocationSource source;
  int x;
  int y;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Get Location Info : {}, " + DecodeEnumName(type) +
           (source == LocationSource::Designation_with_variables ? "({[]},{[]})" : std::format("({},{})", x, y))
    + ColorFormatter::popColor();
  }
};
