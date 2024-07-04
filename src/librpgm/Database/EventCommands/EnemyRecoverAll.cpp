#include "Database/EventCommands/EnemyRecoverAll.hpp"

std::string EnemyRecoverAllCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Recover All" +
         colon.data() + (troop >= 0 ? "#" + std::to_string(troop + 1) : "Entire Troop") + ColorFormatter::popColor();
}
