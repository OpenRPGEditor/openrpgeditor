#include "Database/EventCommands/ChangeTP.hpp"

#include "Database/Database.hpp"

std::string ChangeTPCommand::stringRep(const Database& db) const {
  std::string actorName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value <= 0) {
      actorName = "Entire Party";
    } else {
      actorName = db.actorNameOrId(value);
    }
  } else {
    actorName = db.variableNameOrId(value);
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Variable) {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  } else {
    quantityStr = std::to_string(quantity);
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change TP" + colon.data() +
         actorName + ", " + DecodeEnumName(quantityOp) + " " + quantityStr + ColorFormatter::popColor();
}
