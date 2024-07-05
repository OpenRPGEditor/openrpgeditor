#include "Database/EventCommands/ShowBattleAnimation.hpp"
#include "Database/Database.hpp"

ShowBattleAnimationCommand::ShowBattleAnimationCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(enemy);
  parameters[1].get_to(animation);
  parameters[2].get_to(targetAllEnemies);
}

std::string ShowBattleAnimationCommand::stringRep(const Database& db) const {
  const auto animName = db.animationNameOrId(animation);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Battle Animation" +
         colon.data() + (targetAllEnemies == true ? "Entire Troop, " : std::format("#{}, ", enemy + 1)) + animName +
         ColorFormatter::popColor();
}
