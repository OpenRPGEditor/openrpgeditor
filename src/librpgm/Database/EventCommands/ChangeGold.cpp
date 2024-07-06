#include "Database/EventCommands/ChangeGold.hpp"

#include "Database/Database.hpp"

ChangeGoldCommand::ChangeGoldCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(operation);
  parameters[1].get_to(operandSource);
  parameters[2].get_to(operand);
}

void ChangeGoldCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(operation);
  out.push_back(operandSource);
  out.push_back(operand);
}


std::string ChangeGoldCommand::stringRep(const Database& db) const {
  if (operandSource == QuantityChangeSource::Constant)
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Gold" + colon.data() +
           DecodeEnumName(operation) + std::to_string(operand) + ColorFormatter::popColor();

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Gold" + colon.data() +
         DecodeEnumName(operation) + std::format(" {{{}}} ", db.variableNameOrId(operand)) +
         ColorFormatter::popColor(); // Add variable name at the end
}
