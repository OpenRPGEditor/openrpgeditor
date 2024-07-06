#include "Database/EventCommands/ChangeEnemyState.hpp"

#include "Database/Database.hpp"

ChangeEnemyStateCommand::ChangeEnemyStateCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(enemy);
  parameters[1].get_to(enemyOp);
  parameters[2].get_to(state);
}

void ChangeEnemyStateCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(enemy);
  out.push_back(enemyOp);
  out.push_back(state);
}

std::string ChangeEnemyStateCommand::stringRep(const Database& db) const {
  std::string enemyStr;
  if (enemy < 0) {
    enemyStr = "Entire Troop, ";
  } else {
    enemyStr = std::format("#{}, ", enemy + 1);
  }

  const auto stStr = db.stateNameOrId(state);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Enemy State" +
         colon.data() + enemyStr + DecodeEnumName(enemyOp) + " " + stStr + ColorFormatter::popColor();
}
