#include "Database/EventCommands/JumpToLabel.hpp"

std::string JumpToLabelCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Jump to Label" + colon.data() +
         label + ColorFormatter::popColor();
}
