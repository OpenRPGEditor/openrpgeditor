#include "Database/EventCommands/ControlSwitches.hpp"

#include "Database/Database.hpp"

std::string ControlSwitches::stringRep(const Database& db) const {
  if (start != end) {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Control Switches" +
           colon.data() + std::format("#{:04}..{:04}", start, end) + " = " + DecodeEnumName(turnOff) +
           ColorFormatter::popColor();
  }
  const auto swName = db.system.switche(start);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Control Switches" +
         colon.data() + db.nameAndId(swName, start) + " = " + DecodeEnumName(turnOff) + ColorFormatter::popColor();
}
