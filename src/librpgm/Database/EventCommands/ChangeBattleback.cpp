#include "Database/EventCommands/ChangeBattleback.hpp"

#include "Database/Database.hpp"

std::string ChangeBattlebackCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Battle Back" +
         colon.data() + db.dualImageText(battleBack1Name, battleBack2Name) + ColorFormatter::popColor();
}
