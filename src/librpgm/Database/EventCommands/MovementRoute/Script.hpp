#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementScriptCommand final : IMovementRouteStep {
  MovementScriptCommand() = default;
  explicit MovementScriptCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~MovementScriptCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Script_del_Movement; }
  void serializeParameters(nlohmann::json& out) override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  std::string script;
};
