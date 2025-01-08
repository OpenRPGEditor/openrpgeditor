#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include "Database/MovementRoute.hpp"
#include <format>

struct MovementRouteStepCommand final : IEventCommand {
  MovementRouteStepCommand() = default;
  explicit MovementRouteStepCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~MovementRouteStepCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Movement_Route_Step; }
  std::shared_ptr<IEventCommand> step;

  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<MovementRouteStepCommand>(*this); }

  [[nodiscard]] std::string stringRep(const Database& db) const override { return step->stringRep(db); }
};

struct SetMovementRouteCommand final : IEventCommand {
  SetMovementRouteCommand() = default;
  explicit SetMovementRouteCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~SetMovementRouteCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Movement_Route; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<SetMovementRouteCommand>(*this); }

  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  void addStep(MovementRouteStepCommand* step) { editNodes.emplace_back(step); }

  int character{-1};
  MovementRoute route;
  std::vector<std::shared_ptr<MovementRouteStepCommand>> editNodes;
};