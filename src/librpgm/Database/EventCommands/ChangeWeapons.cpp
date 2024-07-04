#include "Database/EventCommands/ChangeWeapons.hpp"

#include "Database/Database.hpp"

std::string ChangeWeaponsCommmand::stringRep(const Database& db) const {
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
