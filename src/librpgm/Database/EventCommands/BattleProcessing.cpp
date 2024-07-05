#include "Database/EventCommands/BattleProcessing.hpp"

#include "Database/Database.hpp"

BattleProcessingCommand::BattleProcessingCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(type);
  parameters[1].get_to(id);
  parameters[2].get_to(canEscape);
  parameters[3].get_to(canLose);
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
