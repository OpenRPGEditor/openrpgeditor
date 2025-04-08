#include "Database/EventCommands/ReturnToTitle.hpp"

std::string ReturnToTitleCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Return To Title Screen") + ColorFormatter::popColor(colored);
}