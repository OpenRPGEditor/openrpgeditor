#include "Database/EventCommands/ChangeMenuAccess.hpp"

std::string ChangeMenuAccessCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Menu Access" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}
