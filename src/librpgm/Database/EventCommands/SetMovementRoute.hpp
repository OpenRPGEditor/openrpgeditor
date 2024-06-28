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
  [[nodiscard]] std::string stringRep(const Database& db) const override {

    std::string characterName = character == -1 ? "Player" : character == 0 ? "This Event" : "{}";
    std::string stringSuffix = "(";
    stringSuffix += route.repeat == true ? "Repeat" : "";
    stringSuffix += route.skippable == true ?  (route.repeat == true ? ", Skip" : "Skip") : "";
    stringSuffix += route.wait == true ? ((route.repeat == true || route.skippable == true) ? ", Wait" : "") : "Wait";
    stringSuffix += ")";

    std::string moveRoute = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Set Movement Route : " + characterName + ColorFormatter::popColor() +
                            ColorFormatter::getColor(Color::Gray) + stringSuffix + ColorFormatter::popColor();

    for (const auto& t : route.list) {
      if (t->code() != EventCode::Event_Dummy) {
        moveRoute += "\n" + std::string(indent ? *indent : 0, '\t') + " :" + std::string(((t->indent ? *t->indent : 0) + 1), '\t') +
               " : " + t->stringRep(db);
      }
    }
    return moveRoute;

  }

};

struct MovementRouteStepCommand : IEventCommand {
  ~MovementRouteStepCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Movement_Route_Step; }
  int character;
  std::shared_ptr<IEventCommand> step;
};
