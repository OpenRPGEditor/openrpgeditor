#include "Database/EventCommands/ChangeArmors.hpp"

#include "Database/Database.hpp"

std::string ChangeArmorsCommand::stringRep(const Database& db) const {
  const auto armorName = db.armorNameOrId(item);
  std::string oper;
  if (operandSource == QuantityChangeSource::Variable) {
    oper = std::format("{{{}}}", db.system.variable(operand));
  } else {
    oper = std::to_string(operand);
  }
  std::string suffix;
  if (includeEquipment) {
    suffix = ColorFormatter::getColor(Color::Gray) + db.parentheses("Include Equipment") + ColorFormatter::popColor();
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Armors" + colon.data() +
         armorName + " " + DecodeEnumName(operation) + " " + oper + ColorFormatter::popColor() + suffix;
}
