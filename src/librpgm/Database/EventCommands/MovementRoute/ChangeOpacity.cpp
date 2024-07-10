#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"

MovementChangeOpacityCommand::MovementChangeOpacityCommand(const std::optional<int>& indent,
                                                           const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(opacity);
}

void MovementChangeOpacityCommand::serializeParameters(nlohmann::json& out) const { out.push_back(opacity); }

std::string MovementChangeOpacityCommand::stringRep(const Database& db) const {
  return symbol(code()) + std::string("Opacity") + colon.data() + std::to_string(opacity);
}
