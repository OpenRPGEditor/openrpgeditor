#include "Database/EventCommands/ChangeVehicleImage.hpp"

std::string ChangeVehicleImageCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Vehicle Image" +
         colon.data() + DecodeEnumName(vehicle) + ", " + picture + std::format("({})", pictureIndex) +
         ColorFormatter::popColor();
}
