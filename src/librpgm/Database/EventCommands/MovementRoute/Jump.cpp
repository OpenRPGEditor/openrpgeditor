#include "Database/EventCommands/MovementRoute/Jump.hpp"

MovementJumpCommand::MovementJumpCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent) {
  parameters.at(0).get_to(x);
  parameters.at(1).get_to(y);
}

void MovementJumpCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(x);
  out.push_back(y);
}

std::string MovementJumpCommand::stringRep(const Database& db, const bool colored) const { return std::format("{}Jump{}{}, {}", symbol((code())), colon, x, y); }
