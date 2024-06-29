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

  [[nodiscard]] std::string stringRep(const Database& db) const override {

    std::string characterName = character == -1 ? "Player" : character == 0 ? "This Event" : "{}";
    std::string stringSuffix = "(";
    stringSuffix += route.repeat == true ? "Repeat" : "";
    stringSuffix += route.skippable == true ? (route.repeat == true ? ", Skip" : "Skip") : "";
    stringSuffix += route.wait == true ? ((route.repeat == true || route.skippable == true) ? ", Wait" : "") : "Wait";
    stringSuffix += ")";

    std::string moveRoute = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) +
                            "Set Movement Route : " + characterName + ColorFormatter::popColor() +
                            ColorFormatter::getColor(Color::Gray) + stringSuffix + ColorFormatter::popColor();

    for (const auto& t : editNodes) {
      if (t->code() != EventCode::Event_Dummy) {
        moveRoute += "\n" + std::string(indent ? *indent : 0, '\t') + " :" +
                     std::string(((t->indent ? *t->indent : 0) + 1), '\t') + " : " + t->stringRep(db);
      }
    }
    return moveRoute;
  }
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
};
