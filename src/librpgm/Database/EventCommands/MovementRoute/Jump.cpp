#include "Database/EventCommands/MovementRoute/Jump.hpp"

MovementJumpCommand::MovementJumpCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(x);
  parameters[1].get_to(y);
}

void MovementJumpCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(x);
  out.push_back(y);
}

std::string MovementJumpCommand::stringRep(const Database& db) const {
  return std::format("{}Jump{}{}, {}", whiteDiamond, colon, x, y);
}
