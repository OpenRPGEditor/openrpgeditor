#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TransferPlayerCommand : IEventCommand {
  ~TransferPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transfer_Player; }
  TransferMode mode;
  int mapId;
  int x;
  int y;
  Direction direction;
  Fade fade;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    std::string suffix = ColorFormatter::getColor(Color::Gray) + (direction != Direction::Retain ? "(Direction: " + DecodeEnumName(direction) : "");
    suffix += (direction != Direction::Retain ? fade != Fade::Black ? ", Fade: " + DecodeEnumName(fade) + ")" : "(Fade: " + DecodeEnumName(fade) + ")" : "") + ColorFormatter::popColor();

    if (mode == TransferMode::Variable_Designation) {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Transfer Player : {[]} ({[]},{[]})" + suffix;
    }
    else {
      return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Transfer Player : {}" + std::format("({}, {})", x, y) + suffix;
    }
  }
};
