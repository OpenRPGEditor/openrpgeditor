#include "Database/EventCommands/ControlTimer.hpp"

std::string ControlTimer::stringRep(const Database& db) const {
  std::string min;
  std::string sec;

  if (seconds > 59) {
    min = std::to_string(seconds / 60);
    sec = std::to_string(seconds % 60);
  } else {
    min = "0";
    sec = std::to_string(seconds);
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Control Timer" + colon.data() +
         DecodeEnumName(control) + ", " + min + " min " + sec + " sec" + ColorFormatter::popColor();
}
