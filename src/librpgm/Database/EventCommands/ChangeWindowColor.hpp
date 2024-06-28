#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeWindowColorCommand : IEventCommand {
  ~ChangeWindowColorCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Window_Color; }
  int r;
  int g;
  int b;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Change Window Color : " + std::format("({}, {}, {})", r, g, b) + ColorFormatter::popColor();
  }
};
