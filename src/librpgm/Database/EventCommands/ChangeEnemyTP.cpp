#include "Database/EventCommands/ChangeEnemyTP.hpp"

#include "Database/Database.hpp"

ChangeEnemyTPCommand::ChangeEnemyTPCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(enemy);
  parameters[1].get_to(enemyOp);
  parameters[2].get_to(quantitySource);
  parameters[3].get_to(quantity);
}

void ChangeEnemyTPCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(enemy);
  out.push_back(enemyOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
}

std::string ChangeEnemyTPCommand::stringRep(const Database& db) const {
  std::string enemyStr;
  if (enemy < 0) {
    enemyStr = "Entire Troop";
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