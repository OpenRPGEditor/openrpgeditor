#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementSwitchOFFCommand final : IMovementRouteStep {
  MovementSwitchOFFCommand() = default;
  explicit MovementSwitchOFFCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementSwitchOFFCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Switch_OFF; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementSwitchOFFCommand>(*this); }

  int id{1};
};
