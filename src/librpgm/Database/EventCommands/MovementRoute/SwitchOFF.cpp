#include "Database/EventCommands/MovementRoute/SwitchOFF.hpp"
#include "Database/Database.hpp"

MovementSwitchOFFCommand::MovementSwitchOFFCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(id);
}

void MovementSwitchOFFCommand::serializeParameters(nlohmann::json& out) const { out.push_back(id); }

std::string MovementSwitchOFFCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + std::format("{:04}", id);
}
