#include "Database/EventCommands/ChangeState.hpp"

#include "Database/Database.hpp"

ChangeStateCommand::ChangeStateCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(comparison);
  parameters.at(1).get_to(value);
  parameters.at(2).get_to(stateOp);
  parameters.at(3).get_to(state);
}

void ChangeStateCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(comparison);
  out.push_back(value);
  out.push_back(stateOp);
  out.push_back(state);
}

std::string ChangeStateCommand::stringRep(const Database& db) const {
  std::string actorName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value == 0) {
      actorName = "Entire Party";
    } else {
      actorName = db.actorNameOrId(value);
    }

  } else {
    actorName = std::format("{{{}}}", db.variableNameOrId(value));
  }

  std::string stateName = db.stateNameOrId(state);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change State" + colon.data() +
         actorName + ", " + DecodeEnumName(stateOp) + " " + stateName + ColorFormatter::popColor();
}
