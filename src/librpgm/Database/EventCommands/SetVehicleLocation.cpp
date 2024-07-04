#include "Database/EventCommands/SetVehicleLocation.hpp"
#include "Database/Database.hpp"

std::string SetVehicleLocationCommand::stringRep(const Database& db) const {
  const auto prefix = indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) +
                      "Set Vehicle Location" + colon.data() + DecodeEnumName(vehicle) + ",";
  const auto suffix = ColorFormatter::popColor();

  if (mode == TransferMode::Variable_Designation) {
    auto varMap = db.system.variable(mapId);
    auto varX = db.system.variable(x);
    auto varY = db.system.variable(y);
    varMap = varMap.empty() ? std::format("#{:04}", mapId) : varMap;
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return prefix + std::format(" {{{}}} ({{{}}},{{{}}}) ", varMap, varX, varY) + suffix;
  }
  const auto map = db.mapInfos.map(mapId);
  return prefix + std::format(" {} ({}, {})", map->name, x, y) + suffix;
}
