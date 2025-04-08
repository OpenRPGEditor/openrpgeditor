#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include <format>

struct MovementChangeOpacityCommand final : IMovementRouteStep {
  MovementChangeOpacityCommand() = default;
  explicit MovementChangeOpacityCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementChangeOpacityCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Opacity; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementChangeOpacityCommand>(*this); }

  int opacity{255};
};
