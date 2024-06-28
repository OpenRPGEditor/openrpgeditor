#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowPictureCommand : IEventCommand {
  ~ShowPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Picture; }
  int number;
  std::string imageName;
  PictureOrigin origin;
  PictureDesignationSource type;
  int value1; // direct X value or indirect from global variables
  int value2; // direct Y value or indirect from global variables
  int zoomX;
  int zoomY;
  int opacityValue;
  Blend blendMode;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Show Picture : " +
        std::format("#{}, {}, {} ({},{}), ({}%, {}%), {}, {}, {} frames",
          number, imageName, DecodeEnumName(origin), DecodeEnumName(type), value1, value2, zoomX, zoomY, opacityValue, DecodeEnumName(blendMode))
    + ColorFormatter::popColor();
  }
};
