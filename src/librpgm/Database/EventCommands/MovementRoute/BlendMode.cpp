#include "Database/EventCommands/MovementRoute/BlendMode.hpp"

MovementChangeBlendModeCommand::MovementChangeBlendModeCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent) {
  parameters.at(0).get_to(mode);
}

void MovementChangeBlendModeCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(mode); }
