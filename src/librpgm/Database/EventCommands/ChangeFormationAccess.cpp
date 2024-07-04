#include "Database/EventCommands/ChangeFormationAccess.hpp"

std::string ChangeFormationAccessCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Formation Access" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}