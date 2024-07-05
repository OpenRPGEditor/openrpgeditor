#include "Database/EventCommands/MovementRoute/SwitchON.hpp"

MovementSwitchONCommand::MovementSwitchONCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(id);
}

std::string MovementSwitchONCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + std::format("{:04}", id);
}
