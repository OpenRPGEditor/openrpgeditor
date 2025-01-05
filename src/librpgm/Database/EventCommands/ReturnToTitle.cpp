#include "Database/EventCommands/ReturnToTitle.hpp"

std::string ReturnToTitleCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Return To Title Screen" + ColorFormatter::popColor();
}