#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"

MovementChangeOpacityCommand::MovementChangeOpacityCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(opacity);
}

void MovementChangeOpacityCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(opacity); }

std::string MovementChangeOpacityCommand::stringRep(const Database& db, const bool colored) const { return symbol(code()) + std::string("Opacity") + colon.data() + std::to_string(opacity); }
