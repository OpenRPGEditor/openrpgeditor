#include "Database/EventCommands/ChangeEXP.hpp"

#include "Database/Database.hpp"

std::string ChangeEXPCommand::stringRep(const Database& db) const {

  std::string actorName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value <= 0) {
      actorName = "Entire Party";
    } else {
      actorName = db.actorNameOrId(value);
    }
  } else {
    actorName = std::format("{{{}}}", db.variableNameOrId(value));
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Constant) {
    quantityStr = std::to_string(quantity);
  } else {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  }

  std::string suffix;
  if (showLevelUp) {
    suffix = ColorFormatter::getColor(Color::Gray) + " " + db.parentheses("Show Level Up") + ColorFormatter::popColor();
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change EXP" + colon.data() +
         actorName + ", " + DecodeEnumName(quantityOp) + " " + quantityStr + ColorFormatter::popColor() + suffix;
}
