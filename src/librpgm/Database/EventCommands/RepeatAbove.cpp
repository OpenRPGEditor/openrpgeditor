#include "Database/EventCommands/RepeatAbove.hpp"

std::string RepeatAboveCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Repeat Above";
}
