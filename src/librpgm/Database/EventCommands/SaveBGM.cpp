#include "Database/EventCommands/SaveBGM.hpp"

std::string SaveBGMCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Save BGM";
}
