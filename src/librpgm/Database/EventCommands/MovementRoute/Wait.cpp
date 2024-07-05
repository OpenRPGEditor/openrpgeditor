#include "Database/EventCommands/MovementRoute/Wait.hpp"

MovementWaitCommand::MovementWaitCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(duration);
}

std::string MovementWaitCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Wait") + colon.data() + std::to_string(duration) + " frames";
}
