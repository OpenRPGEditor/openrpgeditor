#include "Database/EventCommands/ChangeMP.hpp"
#include "Database/Database.hpp"

std::string ChangeMPCommand::stringRep(const Database& db) const {
  std::string actName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value <= 0) {
      actName = "Entire Party";
    } else {
      actName = db.actorNameOrId(value);
    }
  } else {
    actName = std::format("{{{}}}", db.variableNameOrId(value));
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Constant) {
    quantityStr = std::to_string(quantity);
  } else {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change MP" + colon.data() +
         actName + ", " + DecodeEnumName(quantityOp) + quantityStr + ColorFormatter::popColor();
}
