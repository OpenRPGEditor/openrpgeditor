#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowPictureCommand final : IEventCommand {
  ShowPictureCommand() = default;
  explicit ShowPictureCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ShowPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Picture; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

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
};
