#include "Database/EventCommands/MovementRoute/Speed.hpp"

MovementSpeedCommand::MovementSpeedCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IMovementRouteStep(indent, parameters) { parameters.at(0).get_to(speed); }

void MovementSpeedCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(speed); }

std::string MovementSpeedCommand::stringRep(const Database& db) const { return symbol(code()) + DecodeEnumName(code()) + colon.data() + std::to_string(speed); }
