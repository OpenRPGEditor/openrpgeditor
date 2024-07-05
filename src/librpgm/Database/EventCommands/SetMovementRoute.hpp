#pragma once
#include "Database/EventCommands/MovementRoute/IMovementRouteStep.hpp"
#include "Database/MovementRoute.hpp"
#include <format>

struct MovementRouteStepCommand final : IEventCommand {
  MovementRouteStepCommand() = default;
  explicit MovementRouteStepCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~MovementRouteStepCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Movement_Route_Step; }
  std::shared_ptr<IEventCommand> step;

  void serializeParameters(nlohmann::json& out) override {
    const auto code = step->code();
    const bool doParameters = code == EventCode::Change_Blend_Mode || code == EventCode::Change_Image ||
                              code == EventCode::Change_Opacity || code == EventCode::Frequency ||
                              code == EventCode::Speed || code == EventCode::Play_SE_del_Movement ||
                              code == EventCode::Wait_del_Movement || code == EventCode::Script_del_Movement;
    step->serialize(out.emplace_back(), step->code() != EventCode::Event_Dummy, doParameters);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override { return step->stringRep(db); }
};

struct SetMovementRouteCommand final : IEventCommand {
  SetMovementRouteCommand() = default;
  explicit SetMovementRouteCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(character);
    parameters[1].get_to(route);
  }
  ~SetMovementRouteCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Movement_Route; }
  void serializeParameters(nlohmann::json& out) override {
    out.push_back(character);
    out.push_back(route);
  }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  void addStep(MovementRouteStepCommand* step) { editNodes.emplace_back(step); }
  int character;
  MovementRoute route;
  std::vector<std::shared_ptr<MovementRouteStepCommand>> editNodes;
};