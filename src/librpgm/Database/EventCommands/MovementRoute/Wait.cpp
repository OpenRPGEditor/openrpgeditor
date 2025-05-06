#include "Database/EventCommands/MovementRoute/Wait.hpp"

MovementWaitCommand::MovementWaitCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent) {
  parameters.at(0).get_to(duration);
}

void MovementWaitCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(duration); }

std::string MovementWaitCommand::stringRep(const Database& db, const bool colored) const { return symbol(code()) + tr("Wait") + colon.data() + std::to_string(duration) + " frames"; }
