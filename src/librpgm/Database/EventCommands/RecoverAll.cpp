#include "Database/EventCommands/RecoverAll.hpp"

#include "Database/Database.hpp"

std::string RecoverAllCommand::stringRep(const Database& db) const {
  std::string source = comparison == ActorComparisonSource::Fixed
                           ? value == 0 ? "Entire Party" : db.actors.actor(value)->name
                           : "{" + db.system.variable(value) + "}";
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Recover All" + colon.data() +
         source + ColorFormatter::popColor();
}
