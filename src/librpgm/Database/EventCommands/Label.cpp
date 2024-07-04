#include "Database/EventCommands/Label.hpp"

std::string LabelCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Label" + colon.data() + label +
         ColorFormatter::popColor();
}
