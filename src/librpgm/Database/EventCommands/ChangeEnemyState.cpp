#include "Database/EventCommands/ChangeEnemyState.hpp"

#include "Database/Database.hpp"

ChangeEnemyStateCommand::ChangeEnemyStateCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(troopMember);
  parameters.at(1).get_to(troopMemberOp);
  parameters.at(2).get_to(state);
}

void ChangeEnemyStateCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(troopMember);
  out.push_back(troopMemberOp);
  out.push_back(state);
}

std::string ChangeEnemyStateCommand::stringRep(const Database& db, const bool colored) const {
  std::string enemyStr;
  if (troopMember < 0) {
    enemyStr = tr("Entire Troop") + ", ";
  } else {
    enemyStr = std::format("#{}, ", troopMember + 1);
  }

  const auto stStr = db.stateNameOrId(state);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Enemy State") + colon.data() + enemyStr + DecodeEnumName(troopMemberOp) + " " + stStr +
         ColorFormatter::popColor(colored);
}
