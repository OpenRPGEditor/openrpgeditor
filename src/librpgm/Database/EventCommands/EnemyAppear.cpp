#include "Database/EventCommands/EnemyAppear.hpp"

std::string EnemyAppearCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Appear" + colon.data() +
         "#" + std::to_string(enemy + 1) + ColorFormatter::popColor();
}
