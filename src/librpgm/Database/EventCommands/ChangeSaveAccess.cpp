#include "Database/EventCommands/ChangeSaveAccess.hpp"

std::string ChangeSaveAccessCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Save Access" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}
