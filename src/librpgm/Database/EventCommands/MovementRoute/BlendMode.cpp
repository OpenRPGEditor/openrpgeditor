#include "Database/EventCommands/MovementRoute/BlendMode.hpp"

MovementChangeBlendModeCommand::MovementChangeBlendModeCommand(const std::optional<int>& indent,
                                                               const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(mode);
}

void MovementChangeBlendModeCommand::serializeParameters(nlohmann::json& out) const { out.push_back(mode); }
