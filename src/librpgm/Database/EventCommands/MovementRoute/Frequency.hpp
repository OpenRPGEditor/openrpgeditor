#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementFrequencyCommand final : IMovementRouteStep {
  MovementFrequencyCommand() = default;
  explicit MovementFrequencyCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementFrequencyCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Frequency; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementFrequencyCommand>(*this); }

  int frequency{3};
};
