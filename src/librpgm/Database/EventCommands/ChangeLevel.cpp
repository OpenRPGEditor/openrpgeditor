#include "Database/EventCommands/ChangeLevel.hpp"

#include "Database/Database.hpp"

std::string ChangeLevelCommand::stringRep(const Database& db) const {
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

  std::string suffix;
  if (showLevelUp) {
    suffix = ColorFormatter::getColor(Color::Gray) + " (Show Level Up)" + ColorFormatter::popColor();
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Level" + colon.data() +
         actName + ", " + DecodeEnumName(quantityOp) + " " + quantityStr + ColorFormatter::popColor() + suffix;
}
