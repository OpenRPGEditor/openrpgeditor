#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutScreenCommand : IEventCommand {
  ~FadeoutScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fade_Out_Screen; }

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Fadeout Screen"
      + ColorFormatter::popColor();
  }
};
