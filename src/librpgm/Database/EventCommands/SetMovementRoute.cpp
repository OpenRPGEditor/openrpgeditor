#include "Database/EventCommands/SetMovementRoute.hpp"
#include "Database/Database.hpp"

#include <iostream>

std::string SetMovementRouteCommand::stringRep(const Database& db) const {
  const auto name = DecodeEnumName(code());
  auto map = db.mapInfos.currentMap();
  const Event* event = map ? map->event(character) : nullptr;
  std::string characterName = character == -1  ? "Player"
                              : character == 0 ? "This Event"
                              : event          ? event->name
                                               : std::to_string(character);
  ;
  std::string stringSuffix = "";
  stringSuffix += route.repeat == true ? "(Repeat" : "(";
  stringSuffix += route.skippable == true ? (route.repeat == true ? ", Skip" : "Skip") : "";
  stringSuffix += route.wait == true ? ((route.repeat == true || route.skippable == true) ? ", Wait)" : "Wait)") : ")";

  std::string moveRoute = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + name +
                          colon.data() + characterName + ColorFormatter::popColor() +
                          ColorFormatter::getColor(Color::Gray) + stringSuffix + ColorFormatter::popColor();

  const std::string stepColor = ColorFormatter::getColorCode(EventCode::Movement_Route_Step);
  const std::string stepSymbol = symbol(EventCode::Movement_Route_Step);
  const std::string stepSpacing = std::string(name.length(), ' ');
  const std::string popColor = ColorFormatter::popColor();
  for (const auto& t : editNodes) {
    if (t->code() != EventCode::Event_Dummy) {
      moveRoute += "\n" + indentText(t->indent) + stepSymbol + stepColor + stepSpacing + colon.data() +
                   t->stringRep(db) + popColor;
    }
  }
  return moveRoute;
}
