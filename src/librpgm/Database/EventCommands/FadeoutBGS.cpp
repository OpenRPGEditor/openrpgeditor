#include "Database/EventCommands/FadeoutBGS.hpp"

std::string FadeoutBGS::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Fadeout BGS" + colon.data() +
         std::to_string(duration) + " seconds" + ColorFormatter::popColor();
}
