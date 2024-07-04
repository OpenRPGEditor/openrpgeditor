#include "Database/EventCommands/ChangeWindowColor.hpp"

std::string ChangeWindowColorCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Window Color" +
         colon.data() + std::format("({}, {}, {})", r, g, b) + ColorFormatter::popColor();
}