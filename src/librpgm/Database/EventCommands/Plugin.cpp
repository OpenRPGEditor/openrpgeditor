#include "Database/EventCommands/Plugin.hpp"

std::string PluginCommandMV::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Plugin Command" + colon.data() +
         command + ColorFormatter::popColor();
}
