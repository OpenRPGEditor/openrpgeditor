#include "Database/EventCommands/ChangeEnemyHP.hpp"

#include "Database/Database.hpp"

std::string ChangeEnemyHPCommand::stringRep(const Database& db) const {
  std::string enemyStr;
  if (enemy < 0) {
    enemyStr = "Entire Troop, ";
  } else {
    enemyStr = std::format("#{}, ", enemy + 1);
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Variable) {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  } else {
    quantityStr = std::format("{}", quantity);
  }

  std::string suffix;
  if (allowKnockOut) {
    suffix =
        ColorFormatter::getColor(Color::Gray) + " " + db.parentheses("Allow Knockout") + ColorFormatter::popColor();
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Enemy HP" + colon.data() +
         enemyStr + DecodeEnumName(enemyOp) + " " + quantityStr + ColorFormatter::popColor() + suffix;
}
