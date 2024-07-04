#include "Database/EventCommands/ChangeState.hpp"

#include "Database/Database.hpp"

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
