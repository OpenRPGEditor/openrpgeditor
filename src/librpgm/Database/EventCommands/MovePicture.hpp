#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct MovePictureCommand final : IEventCommand {
  MovePictureCommand() = default;
  explicit MovePictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Move_Picture; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovePictureCommand>(*this); }

  int picture{1};
  PictureOrigin origin = PictureOrigin::Upper_Left;
  PictureDesignationSource pictureLocation = PictureDesignationSource::Direct_designation;
  int x{1};
  int y{1};
  int width{100};
  int height{100};
  int opacity{255};
  Blend blendMode = Blend::Normal;
  int duration{60};
  bool waitForCompletion{false};
};
