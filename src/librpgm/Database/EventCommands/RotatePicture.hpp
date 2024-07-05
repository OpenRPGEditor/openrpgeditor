#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RotatePictureCommand final : IEventCommand {
  RotatePictureCommand() = default;
  explicit RotatePictureCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~RotatePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Rotate_Picture; }
  int picture;
  int rotation;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
