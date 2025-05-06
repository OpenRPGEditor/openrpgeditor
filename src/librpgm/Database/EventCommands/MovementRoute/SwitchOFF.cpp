#include "Database/EventCommands/MovementRoute/SwitchOFF.hpp"
#include "Database/Database.hpp"

MovementSwitchOFFCommand::MovementSwitchOFFCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent) {
  parameters.at(0).get_to(id);
}

void MovementSwitchOFFCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(id); }

std::string MovementSwitchOFFCommand::stringRep(const Database& db, const bool colored) const { return symbol(code()) + DecodeEnumName(code()) + colon.data() + std::format("{:04}", id); }
