#include "Database/EventCommands/EnemyTransform.hpp"
#include "Database/Database.hpp"

std::string EnemyTransformCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Transform" + colon.data() +
         " #" + std::to_string(enemy + 1) + ", " + db.enemies.enemy(transform)->name + ColorFormatter::popColor();
}
