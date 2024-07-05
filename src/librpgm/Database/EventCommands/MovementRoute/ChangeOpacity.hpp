#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementChangeOpacityCommand final : IMovementRouteStep {
  ~MovementChangeOpacityCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Opacity; }

  int opacity;

  void serializeParameters(nlohmann::json& out) override { out.push_back(opacity); }

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
