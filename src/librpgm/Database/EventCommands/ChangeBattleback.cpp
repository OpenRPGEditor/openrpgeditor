#include "Database/EventCommands/ChangeBattleback.hpp"

#include "Database/Database.hpp"

ChangeBattlebackCommand::ChangeBattlebackCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(battleBack1Name);
  parameters[1].get_to(battleBack2Name);
}

std::string ChangeBattlebackCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Battle Back" +
         colon.data() + db.dualImageText(battleBack1Name, battleBack2Name) + ColorFormatter::popColor();
}
