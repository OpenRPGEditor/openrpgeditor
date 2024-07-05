#include "Database/EventCommands/ChangeItems.hpp"

#include "Database/Database.hpp"

std::string ChangeItemsCommand::stringRep(const Database& db) const {
  std::string itemName = db.itemNameOrId(item);
  std::string operandStr;
  if (operandSource == QuantityChangeSource::Constant) {
    operandStr = std::to_string(operand);
  } else {
    operandStr = std::format("{{{}}}", db.variableNameOrId(operand));
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Items" + colon.data() +
         itemName + " " + DecodeEnumName(operation) + " " + operandStr + ColorFormatter::popColor();
}
