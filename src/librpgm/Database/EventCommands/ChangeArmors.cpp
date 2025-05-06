#include "Database/EventCommands/ChangeArmors.hpp"

#include "Database/Database.hpp"

ChangeArmorsCommand::ChangeArmorsCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(item);
  parameters.at(1).get_to(operation);
  parameters.at(2).get_to(operandSource);
  parameters.at(3).get_to(operand);
  parameters.at(4).get_to(includeEquipment);
}

void ChangeArmorsCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(item);
  out.push_back(operand);
  out.push_back(operandSource);
  out.push_back(operand);
  out.push_back(includeEquipment);
}

std::string ChangeArmorsCommand::stringRep(const Database& db, const bool colored) const {
  const auto armorName = db.armorNameOrId(item);
  std::string oper;
  if (operandSource == QuantityChangeSource::Variable) {
    oper = std::format("{{{}}}", db.system.variable(operand));
  } else {
    oper = std::to_string(operand);
  }
  std::string suffix;
  if (includeEquipment) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + db.parentheses(trNOOP("Include Equipment")) + ColorFormatter::popColor(colored);
  }

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Armors") + colon.data() + armorName + " " + DecodeEnumName(operation) + " " + oper +
         ColorFormatter::popColor(colored) + suffix;
}
