#include "Database/EventCommands/MovementRoute/SwitchOFF.hpp"
#include "Database/Database.hpp"

MovementSwitchOFFCommand::MovementSwitchOFFCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(id);
}

std::string MovementSwitchOFFCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + std::format("{:04}", id);
}
