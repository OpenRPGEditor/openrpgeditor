#include "Database/EventCommands/MovementRoute/Frequency.hpp"

MovementFrequencyCommand::MovementFrequencyCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(frequency);
}
void MovementFrequencyCommand::serializeParameters(nlohmann::json& out) const { out.push_back(frequency); }

std::string MovementFrequencyCommand::stringRep(const Database& db) const {
  return symbol(code()) + std::string("Frequency") + colon.data() + std::to_string(frequency);
}
