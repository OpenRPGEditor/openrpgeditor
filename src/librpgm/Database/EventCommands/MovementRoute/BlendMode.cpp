#include "Database/EventCommands/MovementRoute/BlendMode.hpp"

MovementChangeBlendModeCommand::MovementChangeBlendModeCommand(const std::optional<int>& indent,
                                                               nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(mode);
}

void MovementChangeBlendModeCommand::serializeParameters(nlohmann::json& out) { out.push_back(mode); }
