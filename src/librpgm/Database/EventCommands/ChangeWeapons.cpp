#include "Database/EventCommands/ChangeWeapons.hpp"

#include "Database/Database.hpp"

ChangeWeaponsCommand::ChangeWeaponsCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(item);
  parameters[1].get_to(operation);
  parameters[2].get_to(operandSource);
  parameters[3].get_to(operand);
  parameters[4].get_to(includeEquipment);
}

void ChangeWeaponsCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(item);
  out.push_back(operation);
  out.push_back(operandSource);
  out.push_back(operand);
  out.push_back(includeEquipment);
}

std::string ChangeWeaponsCommand::stringRep(const Database& db) const {
  const auto var = db.system.variable(operand);
  const auto wp = db.weapons.weapon(item);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Weapons" + colon.data() +
         (wp && !wp->name.empty() ? wp->name : std::format("#{:04}", item)) + DecodeEnumName(operation) +
         (operandSource == QuantityChangeSource::Constant ? std::to_string(operand)
          : !var.empty()                                  ? var
                                                          : std::format("#{:04}", operand)) +
         ColorFormatter::popColor() +
         (includeEquipment == true ? ColorFormatter::getColor(Color::Gray) + "(Include Equipment)" : "");
}
