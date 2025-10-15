#include "Database/EventCommands/SetVehicleLocation.hpp"
#include "Database/Database.hpp"

SetVehicleLocationCommand::SetVehicleLocationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(vehicle);
  parameters.at(1).get_to(mode);
  parameters.at(2).get_to(mapId);
  parameters.at(3).get_to(x);
  parameters.at(4).get_to(y);
}

void SetVehicleLocationCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(vehicle);
  out.push_back(mode);
  out.push_back(mapId);
  out.push_back(x);
  out.push_back(y);
}

std::string SetVehicleLocationCommand::stringRep(const Database& db, const bool colored) const {
  const auto prefix = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Set Vehicle Location") + colon.data() + DecodeEnumName(vehicle) + ",";
  const auto suffix = ColorFormatter::popColor(colored);

  if (mode == TransferMode::Variable_Designation) {
    auto varMap = db.system->variable(mapId);
    auto varX = db.system->variable(x);
    auto varY = db.system->variable(y);
    varMap = varMap.empty() ? std::format("#{:04}", mapId) : varMap;
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return prefix + std::format(" {{{}}} ({{{}}},{{{}}}) ", varMap, varX, varY) + suffix;
  }
  const auto map = db.mapInfos->map(mapId);
  return prefix + std::format(" {} ({}, {})", map->name(), x, y) + suffix;
}
