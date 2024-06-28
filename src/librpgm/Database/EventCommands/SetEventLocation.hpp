#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetEventLocationCommand : IEventCommand {
  ~SetEventLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Event_Location; }
  TransferMode mode;
  int event;
  int x;
  int y;
  Direction direction;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    std::string prefix = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Event Location : " + (event > 0 ? "{}" : "This Event");
    std::string suffix = "(Direction : " + DecodeEnumName(direction) + " )" + ColorFormatter::popColor();

    if (mode == TransferMode::Variable_Designation) {
      return prefix + ", ([],[])" + suffix;
    }
    if (mode == TransferMode::Exchange_With_Another_Event) {
      return prefix + ", Exchange with " + (event > 0 ? "<>" : "This Event") + suffix;
    }
    return prefix + std::format(", ({}, {})", x, y) + suffix;
  }
};
