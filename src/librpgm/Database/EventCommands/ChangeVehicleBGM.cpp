#include "Database/EventCommands/ChangeVehicleBGM.hpp"

ChangeVehicleBGMCommand::ChangeVehicleBGMCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(vehicle);
  parameters[1].get_to(bgm);
}

void ChangeVehicleBGMCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(vehicle);
  out.push_back(bgm);
}

std::string ChangeVehicleBGMCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Vehicle BGM" +
         colon.data() + DecodeEnumName(vehicle) + ", " + (bgm.name == "" ? "None" : bgm.name) + " " +
         std::format("({}, {}, {})", bgm.volume, bgm.pitch, bgm.pan) + ColorFormatter::popColor();
}
