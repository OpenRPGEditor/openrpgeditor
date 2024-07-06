#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RotatePictureCommand final : IEventCommand {
  RotatePictureCommand() = default;
  explicit RotatePictureCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~RotatePictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Rotate_Picture; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int picture;
  int rotation;
};
