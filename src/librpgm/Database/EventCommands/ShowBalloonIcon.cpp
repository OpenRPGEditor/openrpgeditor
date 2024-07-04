#include "Database/EventCommands/ShowBalloonIcon.hpp"

#include "Database/Database.hpp"

std::string ShowBalloonIconCommand::stringRep(const Database& db) const {
  const auto map = db.mapInfos.currentMap();
  const auto ev = map ? map->event(id) : nullptr;
  auto evName = ev && !ev->name.empty() ? ev->name : std::format("#{:03}", id);
  evName = (id > 0 ? " " + evName + ", " : id == -2 ? " Player, " : " This Event, ");
  const auto suffix =
      waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor() : "";
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Balloon Icon" +
         colon.data() + evName + DecodeEnumName(index) + ColorFormatter::popColor() + suffix;
}
