#include "Database/EventCommands/ChangeHP.hpp"

#include "Database/Database.hpp"

std::string ChangeHPCommand::stringRep(const Database& db) const {
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
    actName = std::format("{{{}}}", db.variableNameOrId(quantity));
  }

  std::string suffix;
  if (allowKnockout) {
    suffix =
        ColorFormatter::getColor(Color::Gray) + " " + db.parentheses("Allow Knockout") + ColorFormatter::popColor();
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change HP" + colon.data() +
         actName + ", " + DecodeEnumName(quantityOp) + " " + quantityStr + ColorFormatter::popColor() + suffix;
}
