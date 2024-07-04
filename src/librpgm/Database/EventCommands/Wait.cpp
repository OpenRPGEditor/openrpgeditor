#include "Database/EventCommands/Wait.hpp"

std::string WaitCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) +
         "Wait : " + std::to_string(duration) + " frames" + ColorFormatter::popColor();
}
