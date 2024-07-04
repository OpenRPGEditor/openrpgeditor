#include "Database/EventCommands/ChangeBattleBGM.hpp"

#include "Database/Database.hpp"

std::string ChangeBattleBGMCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Battle BGM" +
         colon.data() + db.audioText(bgm) + ColorFormatter::popColor();
}
