#include "Database/EventCommands/MovementRoute/Speed.hpp"

MovementSpeedCommand::MovementSpeedCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(speed);
}

std::string MovementSpeedCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + std::to_string(speed);
}
