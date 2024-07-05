#include "Database/EventCommands/ChangeGold.hpp"

#include "Database/Database.hpp"

std::string ChangeGoldCommand::stringRep(const Database& db) const {
  if (operandSource == QuantityChangeSource::Constant)
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Gold" + colon.data() +
           DecodeEnumName(operation) + std::to_string(operand) + ColorFormatter::popColor();

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Gold" + colon.data() +
         DecodeEnumName(operation) + std::format(" {{{}}} ", db.variableNameOrId(operand)) +
         ColorFormatter::popColor(); // Add variable name at the end
}
