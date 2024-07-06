#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovePictureCommand final : IEventCommand {
  MovePictureCommand() = default;
  explicit MovePictureCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~MovePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Picture; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

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
};
