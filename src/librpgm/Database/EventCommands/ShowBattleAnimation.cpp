#include "Database/EventCommands/ShowBattleAnimation.hpp"
#include "Database/Database.hpp"

ShowBattleAnimationCommand::ShowBattleAnimationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(enemy);
  parameters.at(1).get_to(animation);
  parameters.at(2).get_to(targetAllEnemies);
}

void ShowBattleAnimationCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(enemy);
  out.push_back(animation);
  out.push_back(targetAllEnemies);
}

std::string ShowBattleAnimationCommand::stringRep(const Database& db, const bool colored) const {
  const auto animName = db.animationNameOrId(animation);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Show Battle Animation") + colon.data() +
         (targetAllEnemies == true ? tr("Entire Troop") + ", " : std::format("#{}, ", enemy)) + animName + ColorFormatter::popColor(colored);
}
