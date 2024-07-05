#include "Database/EventCommands/ChangeState.hpp"

#include "Database/Database.hpp"

ChangeStateCommand::ChangeStateCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(comparison);
  parameters[1].get_to(value);
  parameters[2].get_to(stateOp);
  parameters[3].get_to(state);
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
