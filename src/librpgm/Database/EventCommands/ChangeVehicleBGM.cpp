#include "Database/EventCommands/ChangeVehicleBGM.hpp"

#include "Database/Database.hpp"

ChangeVehicleBGMCommand::ChangeVehicleBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  parameters.at(0).get_to(vehicle);
  parameters.at(1).get_to(bgm);
}

void ChangeVehicleBGMCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(vehicle);
  out.push_back(bgm);
}

std::string ChangeVehicleBGMCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Vehicle BGM" + colon.data() + DecodeEnumName(vehicle) + ", " + Database::audioText(bgm) +
         ColorFormatter::popColor();
}
