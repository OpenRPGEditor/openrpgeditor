#include "Database/EventCommands/GetOnOffVehicle.hpp"

std::string GetOnOffVehicleCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Get on/off Vehicle") + ColorFormatter::popColor(colored);
}
