#include "Database/EventCommands/ChangeVictoryME.hpp"

std::string ChangeVictoryMECommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Victory ME" +
         colon.data() + (me.name == "" ? "None" : me.name) + " " +
         std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
}
