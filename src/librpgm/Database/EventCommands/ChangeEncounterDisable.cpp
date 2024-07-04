#include "Database/EventCommands/ChangeEncounterDisable.hpp"

std::string ChangeEncounterDisableCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Encounter" +
         colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}
