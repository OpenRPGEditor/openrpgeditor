#include "Database/EventCommands/BattleProcessing.hpp"

#include "Database/Database.hpp"

std::string BattleProcessingCommand::stringRep(const Database& db) const {
  std::string enemy;
  if (type == BattleProcessType::Direct_designation) {
    enemy = db.enemyNameOrId(id);
  } else if (type == BattleProcessType::Designation_with_variables) {
    enemy = std::format("{{{}}}", db.variableNameOrId(id));
  } else {
    enemy = "Same as Random Encounter";
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Battle Processing" +
         colon.data() + enemy + ColorFormatter::popColor();
}
