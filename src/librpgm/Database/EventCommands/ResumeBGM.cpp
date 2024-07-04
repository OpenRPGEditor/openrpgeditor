#include "Database/EventCommands/ResumeBGM.hpp"

std::string ResumeBGMCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Resume BGM";
}
