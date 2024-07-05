#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"

MovementChangeOpacityCommand::MovementChangeOpacityCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(opacity);
}

void MovementChangeOpacityCommand::serializeParameters(nlohmann::json& out) { out.push_back(opacity); }
std::string MovementChangeOpacityCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Opacity") + colon.data() + std::to_string(opacity);
}
