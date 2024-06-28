#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovePictureCommand : IEventCommand {
  ~MovePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Picture; }
  int picture;
  PictureOrigin origin;
  PictureDesignationSource pictureLocation;
  int x;
  int y;
  int width;
  int height;
  int opacity;
  Blend blendMode;
  int duration;
  bool waitForCompletion;

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Move Picture : " +
        std::format("#{}, {} ({},{}), ({}%, {}%), {}, {}, {} frames",
          picture, DecodeEnumName(origin), DecodeEnumName(pictureLocation), x, y, width, height, opacity, DecodeEnumName(blendMode), duration)
    + (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor() : "");
  }
};
