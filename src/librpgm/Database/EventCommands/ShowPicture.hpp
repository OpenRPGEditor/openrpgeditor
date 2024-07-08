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

  int number{1};
  std::string imageName;
  PictureOrigin origin = PictureOrigin::Upper_Left;
  PictureDesignationSource type = PictureDesignationSource::Direct_designation;
  int value1{1}; // direct X value or indirect from global variables
  int value2{1}; // direct Y value or indirect from global variables
  int zoomX{100};
  int zoomY{100};
  int opacityValue{255};
  Blend blendMode = Blend::Normal;
};
