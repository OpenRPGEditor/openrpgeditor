#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct OpenSaveCommand : IEventCommand {
  ~OpenSaveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Save_Screen; }
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Open Save Screen" + ColorFormatter::popColor();
  }
};
