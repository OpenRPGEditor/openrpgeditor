#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementChangeBlendModeCommand final : IMovementRouteStep {
  ~MovementChangeBlendModeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Blend_Mode; }

  Blend mode;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(mode);
  }
};
