#include "Database/EventCommands/MovementRoute/Jump.hpp"

MovementJumpCommand::MovementJumpCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(x);
  parameters[1].get_to(y);
}

std::string MovementJumpCommand::stringRep(const Database& db) const {
  return std::format("{}Jump{}{}, {}", whiteDiamond, colon, x, y);
}
