#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/MovementRoute.hpp"
#include <format>

struct SetMovementRouteCommand : IEventCommand {
  ~SetMovementRouteCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Movement_Route; }
  int character;
  MovementRoute route;
  std::vector<std::shared_ptr<IEventCommand>> editNodes;

  void serializeParameters(nlohmann::json& out) override {
    out.push_back(character);
    out.push_back(route);
  }

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct MovementRouteStepCommand : IEventCommand {
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

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return step->stringRep(db);
  }
};
