#include "Database/EventCommands/MovementRoute/Script.hpp"

MovementScriptCommand::MovementScriptCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent) {
  parameters.at(0).get_to(script);
}

void MovementScriptCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(script); }

std::string MovementScriptCommand::stringRep(const Database& db, const bool colored) const { return symbol(code()) + DecodeEnumName(code()) + colon.data() + script; }
