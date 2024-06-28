#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RotatePictureCommand : IEventCommand {
  ~RotatePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Rotate_Picture; }
  int picture;
  int rotation;

  [[nodiscard]] std::string stringRep() const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Rotate Picture : #" + std::to_string(picture) + ", " + std::to_string(rotation) + ColorFormatter::popColor();
  }
};
