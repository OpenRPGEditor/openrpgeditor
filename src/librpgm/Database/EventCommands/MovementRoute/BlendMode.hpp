#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementChangeBlendModeCommand final : IMovementRouteStep {
  MovementChangeBlendModeCommand() = default;
  explicit MovementChangeBlendModeCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementChangeBlendModeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Blend_Mode; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementChangeBlendModeCommand>(*this); }

  Blend mode = Blend::Normal;
};
