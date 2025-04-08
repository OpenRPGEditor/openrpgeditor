#include "Database/EventCommands/ChangeGold.hpp"

#include "Database/Database.hpp"

ChangeGoldCommand::ChangeGoldCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(operation);
  parameters.at(1).get_to(operandSource);
  parameters.at(2).get_to(operand);
}

void ChangeGoldCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(operation);
  out.push_back(operandSource);
  out.push_back(operand);
}

std::string ChangeGoldCommand::stringRep(const Database& db, const bool colored) const {
  if (operandSource == QuantityChangeSource::Constant)
    return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Gold") + colon.data() + DecodeEnumName(operation) + std::to_string(operand) +
           ColorFormatter::popColor(colored);

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Gold") + colon.data() + DecodeEnumName(operation) +
         std::format(" {{{}}} ", db.variableNameOrId(operand)) + ColorFormatter::popColor(colored); // Add variable name at the end
}
