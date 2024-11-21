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

std::string InputNumberCommand::stringRep(const Database& db) const {
  auto var = db.system.variable(variable);
  var = var.empty() ? std::format("#{:04}", variable) : var;
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Input Number" + colon.data() +
         var + ", " + std::to_string(digits) + (digits > 1 ? " digits" : " digit") + ColorFormatter::popColor();
}
