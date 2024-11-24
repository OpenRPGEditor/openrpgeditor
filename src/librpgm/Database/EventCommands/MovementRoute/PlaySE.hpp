#pragma once
#include "Database/Audio.hpp"
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementPlaySECommand final : IMovementRouteStep {
  MovementPlaySECommand() = default;
  explicit MovementPlaySECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementPlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE_del_Movement; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  Audio se;
};
