#include "Database/EventCommands/InputNumber.hpp"

#include "Database/Database.hpp"

InputNumberCommand::InputNumberCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(variable);
  parameters.at(1).get_to(digits);
}

void InputNumberCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(variable);
  out.push_back(digits);
}

std::string InputNumberCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Input Number") + colon.data() + db.variableNameOrId(variable) + ", " + std::to_string(digits) +
         (digits > 1 ? " digits" : " digit") + ColorFormatter::popColor(colored);
}
