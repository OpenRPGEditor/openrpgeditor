#include "Database/EventCommands/ChangeEnemyMP.hpp"
#include "Database/Database.hpp"

ChangeEnemyMPCommand::ChangeEnemyMPCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(enemy);
  parameters[1].get_to(enemyOp);
  parameters[2].get_to(quantitySource);
  parameters[3].get_to(quantity);
}

std::string ChangeEnemyMPCommand::stringRep(const Database& db) const {
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

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Enemy MP" + colon.data() +
         enemyStr + DecodeEnumName(enemyOp) + " " + quantityStr + ColorFormatter::popColor();
}