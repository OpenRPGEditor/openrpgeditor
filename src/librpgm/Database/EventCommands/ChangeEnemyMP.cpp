#include "Database/EventCommands/ChangeEnemyMP.hpp"
#include "Database/Database.hpp"

ChangeEnemyMPCommand::ChangeEnemyMPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(troopMember);
  parameters.at(1).get_to(troopMemberOp);
  parameters.at(2).get_to(quantitySource);
  parameters.at(3).get_to(quantity);
}

void ChangeEnemyMPCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(troopMember);
  out.push_back(troopMemberOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
}

std::string ChangeEnemyMPCommand::stringRep(const Database& db, const bool colored) const {
  std::string enemyStr = db.troopMemberName(m_troopId, troopMember);
  if (troopMember >= 0) {
    enemyStr = std::format("#{} {}", troopMember + 1, enemyStr);
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Variable) {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  } else {
    quantityStr = std::format("{}", quantity);
  }

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Enemy MP") + colon.data() + enemyStr + DecodeEnumName(troopMemberOp) + " " +
         quantityStr + ColorFormatter::popColor(colored);
}