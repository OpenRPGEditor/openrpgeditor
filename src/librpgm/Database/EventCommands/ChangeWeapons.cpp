#include "Database/EventCommands/ChangeWeapons.hpp"

#include "Database/Database.hpp"

ChangeWeaponsCommand::ChangeWeaponsCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(item);
  parameters.at(1).get_to(operation);
  parameters.at(2).get_to(operandSource);
  parameters.at(3).get_to(operand);
  parameters.at(4).get_to(includeEquipment);
}

void ChangeWeaponsCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(item);
  out.push_back(operation);
  out.push_back(operandSource);
  out.push_back(operand);
  out.push_back(includeEquipment);
}

std::string ChangeWeaponsCommand::stringRep(const Database& db) const {
  const auto var = db.system.variable(operand);
  const auto wp = db.weapons.weapon(item);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Weapons" + colon.data() + db.weaponNameOrId(item) + DecodeEnumName(operation) +
         (operandSource == QuantityChangeSource::Constant ? std::to_string(operand) : db.variableNameOrId(operand)) + ColorFormatter::popColor() +
         (includeEquipment == true ? ColorFormatter::getColor(FormatColor::Gray) + "(Include Equipment)" : "");
}
