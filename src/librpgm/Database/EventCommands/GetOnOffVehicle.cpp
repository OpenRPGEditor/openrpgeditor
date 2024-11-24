#include "Database/EventCommands/GetOnOffVehicle.hpp"

std::string GetOnOffVehicleCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Get on/off Vehicle" + ColorFormatter::popColor();
}
