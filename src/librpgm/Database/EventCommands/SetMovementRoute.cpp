#include "Database/EventCommands/SetMovementRoute.hpp"
#include "Database/CommandParser.hpp"
#include "Database/Database.hpp"

MovementRouteStepCommand::MovementRouteStepCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  CommandParser p;
  step = p.parse(parameters)[0];
}

void MovementRouteStepCommand::serializeParameters(nlohmann::ordered_json& out) const {
  const auto code = step->code();
  const bool doParameters = code == EventCode::Jump || code == EventCode::Change_Blend_Mode || code == EventCode::Change_Image || code == EventCode::Change_Opacity || code == EventCode::Frequency ||
                            code == EventCode::Speed || code == EventCode::Play_SE_del_Movement || code == EventCode::Wait_del_Movement || code == EventCode::Script_del_Movement ||
                            code == EventCode::Switch_ON || code == EventCode::Switch_OFF;
  step->serialize(out.emplace_back(), step->code() != EventCode::Event_Dummy, doParameters);
}

SetMovementRouteCommand::SetMovementRouteCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(character);
  parameters.at(1).get_to(route);
}

void SetMovementRouteCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(character);
  out.push_back(route);
}

std::string SetMovementRouteCommand::stringRep(const Database& db, const bool colored) const {
  const auto name = DecodeEnumName(code());

  std::string stringSuffix;

  if (route.skippable() == true || route.wait() == true || route.repeat() == true) {
    stringSuffix += "(";
    if (route.repeat() == true) {
      stringSuffix += trNOOP("Repeat");
    }
    if (route.skippable() == true) {
      if (route.repeat() == true) {
        stringSuffix += ", ";
      }
      stringSuffix += trNOOP("Skip");
    }
    if (route.wait() == true) {
      if (route.repeat() == true || route.skippable() == true) {
        stringSuffix += ", ";
      }
      stringSuffix += trNOOP("Wait");
    }
    stringSuffix += ")";
  }

  std::string moveRoute = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + name + colon.data() + db.eventNameOrId(character) +
                          ColorFormatter::popColor(colored) + ColorFormatter::getColor(FormatColor::Gray, colored) + stringSuffix + ColorFormatter::popColor(colored);

  const std::string stepColor = ColorFormatter::getColorCode(EventCode::Movement_Route_Step, colored);
  const std::string stepSymbol = symbol(EventCode::Movement_Route_Step);
  const auto stepSpacing = std::string(name.length(), ' ');
  const std::string popColor = ColorFormatter::popColor(colored);
  for (const auto& t : editNodes) {
    if (t->code() != EventCode::Event_Dummy) {
      moveRoute += "\n" + indentText(t->indent()) + stepSymbol + stepColor + stepSpacing + colon.data() + t->stringRep(db) + popColor;
    }
  }
  return moveRoute;
}
