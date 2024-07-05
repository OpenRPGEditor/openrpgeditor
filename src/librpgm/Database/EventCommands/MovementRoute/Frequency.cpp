#include "Database/EventCommands/MovementRoute/Frequency.hpp"

MovementFrequencyCommand::MovementFrequencyCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(frequency);
}

std::string MovementFrequencyCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Frequency") + colon.data() + std::to_string(frequency);
}
