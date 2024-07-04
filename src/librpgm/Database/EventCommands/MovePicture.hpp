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

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
