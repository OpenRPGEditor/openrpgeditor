#include "Database/EventCommands/ChangeMapDisplayName.hpp"

std::string ChangeMapNameDisplayCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Map Name Display" +
         colon.data() + DecodeEnumName(checkIfOn) + ColorFormatter::popColor();
}
