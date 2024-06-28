#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintScreenCommand : IEventCommand {
  ~TintScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Screen; }
  struct {
    int r;
    int g;
    int b;
    int gray;
  } colors;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Tint Screen : " +
        std::format("({},{},{},{}), {} frames",
          colors.r, colors.g, colors.b, colors.gray, duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};
