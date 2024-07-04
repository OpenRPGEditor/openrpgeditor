#include "Database/EventCommands/ShowBattleAnimation.hpp"

#include "Database/Database.hpp"

std::string ShowBattleAnimationCommand::stringRep(const Database& db) const {
  const auto animName = db.animationNameOrId(animation);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Battle Animation" +
         colon.data() + (targetAllEnemies == true ? "Entire Troop, " : std::format("#{}, ", enemy + 1)) + animName +
         ColorFormatter::popColor();
}
