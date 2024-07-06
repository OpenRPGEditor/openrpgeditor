#include "Database/EventCommands/BattleProcessing.hpp"

#include "Database/Database.hpp"

BattleProcessingCommand::BattleProcessingCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(type);
  parameters.at(1).get_to(id);
  parameters.at(2).get_to(canEscape);
  parameters.at(3).get_to(canLose);
}

void BattleProcessingCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(type);
  out.push_back(id);
  out.push_back(canEscape);
  out.push_back(canLose);
}

std::string BattleProcessingCommand::stringRep(const Database& db) const {
  std::string enemy;
  if (type == BattleProcessType::Direct_designation) {
    enemy = db.troopNameOrId(id);
  } else if (type == BattleProcessType::Designation_with_variables) {
    enemy = std::format("{{{}}}", db.variableNameOrId(id));
  } else {
    enemy = "Same as Random Encounter";
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Battle Processing" +
         colon.data() + enemy + ColorFormatter::popColor();
}
