#include "Database/EventCommands/ChangeBattleback.hpp"

#include "Database/Database.hpp"

ChangeBattlebackCommand::ChangeBattlebackCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(battleBack1Name);
  parameters.at(1).get_to(battleBack2Name);
}

void ChangeBattlebackCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(battleBack1Name);
  out.push_back(battleBack2Name);
}

std::string ChangeBattlebackCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Battle Back") + colon.data() + db.dualImageText(battleBack1Name, battleBack2Name) +
         ColorFormatter::popColor(colored);
}
