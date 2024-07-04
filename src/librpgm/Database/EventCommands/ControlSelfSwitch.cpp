#include "Database/EventCommands/ControlSelfSwitch.hpp"

std::string ControlSelfSwitch::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Control Self Switch" +
         colon.data() + selfSw + " = " + DecodeEnumName(turnOff) + ColorFormatter::popColor();
}
