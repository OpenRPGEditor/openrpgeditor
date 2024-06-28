#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeMapNameDisplayCommand : IEventCommand {
  ~ChangeMapNameDisplayCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Map_Name_Display; }
  ValueControl checkIfOn;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Map Name Display : " + (checkIfOn == ValueControl::ON ? "ON" : "OFF") + ColorFormatter::popColor();
  }
};