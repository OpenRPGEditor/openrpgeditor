#include "Database/EventCommands/ShowAnimation.hpp"

#include "Database/Database.hpp"

std::string ShowAnimationCommand::stringRep(const Database& db) const {
  const auto map = db.mapInfos.currentMap();
  const auto ev = map ? map->event(character) : nullptr;
  const auto evName = ev && !ev->name.empty() ? ev->name : std::format("#{:03}", character);
  const auto animName = db.animationNameOrId(animation);

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Animation" + colon.data() +
         (character == -1  ? "Player"
          : character == 0 ? "This  Event"
                           : evName) +
         ", " + animName + " " + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + "(Wait)" + ColorFormatter::popColor()
                                    : "");
}