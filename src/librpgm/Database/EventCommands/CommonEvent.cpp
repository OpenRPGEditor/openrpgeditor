#include "Database/EventCommands/CommonEvent.hpp"

#include "Database/Database.hpp"

std::string CommonEventCommand::stringRep(const Database& db) const {
  const auto commonEvent = db.commonEvents.event(event);
  const auto eventName = commonEvent && !commonEvent->name.empty() ? commonEvent->name : std::format("#{:04}", event);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Common Event" + colon.data() +
         eventName + ColorFormatter::popColor();
}
