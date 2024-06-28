#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowAnimationCommand : IEventCommand {
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  int character;
  int animation;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Show Animation : " + (character == -1 ? "Player" : character == 0 ? "This  Event" : "{}, ")+ ColorFormatter::popColor()
    + (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + "(Wait)" + ColorFormatter::popColor() : "");
  }
};
