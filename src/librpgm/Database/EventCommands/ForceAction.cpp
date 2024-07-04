#include "Database/EventCommands/ForceAction.hpp"

#include "Database/Database.hpp"

std::string ForceActionCommand::stringRep(const Database& db) const {
  const std::string targetString = target == -2   ? "Last Target"
                                   : target == -1 ? "Random"
                                                  : "Index " + std::to_string(target + 1);
  const auto attack = db.skills.skill(skill);
  if (sourceComparison == SubjectComparisonSource::Enemy) {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Force Action" + colon.data() +
           "#" + std::to_string(source + 1) + ", " + attack->name + ", " + targetString + ColorFormatter::popColor();
  }
  const auto actor = db.actors.actor(source);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Force Action" + colon.data() +
         actor->name + ", " + attack->name + ", " + targetString + ColorFormatter::popColor();
}
