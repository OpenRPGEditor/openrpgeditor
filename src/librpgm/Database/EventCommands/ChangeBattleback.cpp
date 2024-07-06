#include "Database/EventCommands/ChangeBattleback.hpp"

#include "Database/Database.hpp"

ChangeBattlebackCommand::ChangeBattlebackCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(battleBack1Name);
  parameters.at(1).get_to(battleBack2Name);
}

void ChangeBattlebackCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(battleBack1Name);
  out.push_back(battleBack2Name);
}

std::string ChangeBattlebackCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Battle Back" +
         colon.data() + db.dualImageText(battleBack1Name, battleBack2Name) + ColorFormatter::popColor();
}
