#include "Database/EventCommands/NameInput.hpp"

#include "Database/Database.hpp"

std::string NameInputCommand::stringRep(const Database& db) const {
  const auto act = db.actors.actor(actorId);

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Name Input Processing" +
         colon.data() + act->name + ", " + std::to_string(maxChar) + (maxChar > 1 ? " characters" : " character") +
         ColorFormatter::popColor();
}
