#include "Database/EventCommands/ChangeEnemyTP.hpp"

#include "Database/Database.hpp"

std::string ChangeEnemyTPCommand::stringRep(const Database& db) const {
  std::string enemyStr;
  if (enemy < 0) {
    enemyStr = "Entire Troop, ";
  } else {
    enemyStr = std::format("#{}", enemy + 1);
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Variable) {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  } else {
    quantityStr = std::format("{}", quantity);
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Enemy TP" + colon.data() +
         enemyStr + ", " + DecodeEnumName(enemyOp) + " " + quantityStr + ColorFormatter::popColor();
}