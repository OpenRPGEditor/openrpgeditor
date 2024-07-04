#include "Database/EventCommands/ChangeTransparency.hpp"

std::string ChangeTransparencyCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Transparency" +
         colon.data() + DecodeEnumName(transparency) + ColorFormatter::popColor();
}