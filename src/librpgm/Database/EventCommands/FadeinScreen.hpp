#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeinScreenCommand : IEventCommand {
  ~FadeinScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_In_Screen; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadein Screen"
      + ColorFormatter::popColor();
  }
};
