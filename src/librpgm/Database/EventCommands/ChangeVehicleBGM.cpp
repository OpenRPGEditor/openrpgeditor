#include "Database/EventCommands/ChangeVehicleBGM.hpp"

std::string ChangeVehicleBGMCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Vehicle BGM" +
         colon.data() + DecodeEnumName(vehicle) + ", " + (me.name == "" ? "None" : me.name) + " " +
         std::format("({}, {}, {})", me.volume, me.pitch, me.pan) + ColorFormatter::popColor();
}
