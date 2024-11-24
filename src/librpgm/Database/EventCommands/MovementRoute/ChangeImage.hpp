#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementChangeImageCommand final : IMovementRouteStep {
  MovementChangeImageCommand() = default;
  explicit MovementChangeImageCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementChangeImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Image; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  std::string image;
  int character{0};
};
