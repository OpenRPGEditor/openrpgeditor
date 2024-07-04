#include "Database/EventCommands/FadeoutBGM.hpp"

std::string FadeoutBGM::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Fadeout BGM" + colon.data() +
         std::to_string(duration) + " seconds" + ColorFormatter::popColor();
}