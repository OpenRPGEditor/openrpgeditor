#include "Database/EventCommands/SetMovementRoute.hpp"
#include "Database/CommandParser.hpp"
#include "Database/Database.hpp"

MovementRouteStepCommand::MovementRouteStepCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  CommandParser p;
  step = p.parse(parameters)[0];
}

void MovementRouteStepCommand::serializeParameters(nlohmann::json& out) const {
  const auto code = step->code();
  const bool doParameters =
      code == EventCode::Jump || code == EventCode::Change_Blend_Mode || code == EventCode::Change_Image ||
      code == EventCode::Change_Opacity || code == EventCode::Frequency || code == EventCode::Speed ||
      code == EventCode::Play_SE_del_Movement || code == EventCode::Wait_del_Movement ||
      code == EventCode::Script_del_Movement || code == EventCode::Switch_ON || code == EventCode::Switch_OFF;
  step->serialize(out.emplace_back(), step->code() != EventCode::Event_Dummy, doParameters);
}

SetMovementRouteCommand::SetMovementRouteCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(character);
  parameters[1].get_to(route);
}

void SetMovementRouteCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(character);
  out.push_back(route);
}

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
