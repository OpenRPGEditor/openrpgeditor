#include "Database/EventCommands/ControlSelfSwitch.hpp"

std::string ControlSelfSwitchCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Control Self Switch" +
         colon.data() + selfSw + " = " + DecodeEnumName(turnOff) + ColorFormatter::popColor();
}
