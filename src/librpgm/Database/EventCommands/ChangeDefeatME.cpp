#include "Database/EventCommands/ChangeDefeatME.hpp"

#include "Database/Database.hpp"

std::string ChangeDefeatMECommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Defeat ME" +
         colon.data() + db.audioText(me) + ColorFormatter::popColor();
}
