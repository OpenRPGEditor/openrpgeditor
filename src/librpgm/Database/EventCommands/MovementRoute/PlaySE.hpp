#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include "Database/Audio.hpp"
#include <format>

struct MovementPlaySECommand final : IMovementRouteStep {
  MovementPlaySECommand() = default;
  explicit MovementPlaySECommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  Audio se;
};
