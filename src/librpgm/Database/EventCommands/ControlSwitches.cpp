#include "Database/EventCommands/ControlSwitches.hpp"

#include "Database/Database.hpp"

ControlSwitches::ControlSwitches(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(start);
  parameters.at(1).get_to(end);
  parameters.at(2).get_to(turnOff); // It's inverted because why the fuck not
}

void ControlSwitches::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(start);
  out.push_back(end);
  out.push_back(turnOff);
}

std::string ControlSwitches::stringRep(const Database& db, const bool colored) const {
  if (start != end) {
    return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Control Switches") + colon.data() + std::format("#{:04}..{:04}", start, end) + " = " +
           DecodeEnumName(turnOff) + ColorFormatter::popColor(colored);
  }
  const auto swName = db.system.switche(start);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Control Switches") + colon.data() + db.nameAndId(swName, start) + " = " +
         DecodeEnumName(turnOff) + ColorFormatter::popColor(colored);
}
