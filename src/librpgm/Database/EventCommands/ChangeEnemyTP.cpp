#include "Database/EventCommands/ChangeEnemyTP.hpp"

#include "Database/Database.hpp"

ChangeEnemyTPCommand::ChangeEnemyTPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(enemy);
  parameters.at(1).get_to(enemyOp);
  parameters.at(2).get_to(quantitySource);
  parameters.at(3).get_to(quantity);
}

void ChangeEnemyTPCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(enemy);
  out.push_back(enemyOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
}

std::string ChangeEnemyTPCommand::stringRep(const Database& db, const bool colored) const {
  std::string enemyStr;
  if (enemy < 0) {
    enemyStr = trNOOP("Entire Troop");
  } else {
    enemyStr = std::format("#{}", enemy + 1);
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Variable) {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  } else {
    quantityStr = std::format("{}", quantity);
  }

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Enemy TP") + colon.data() + enemyStr + ", " + DecodeEnumName(enemyOp) + " " +
         quantityStr + ColorFormatter::popColor(colored);
}