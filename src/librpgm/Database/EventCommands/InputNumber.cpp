#include "Database/EventCommands/InputNumber.hpp"

#include "Database/Database.hpp"

std::string InputNumberCommand::stringRep(const Database& db) const {
  auto var = db.system.variable(variable);
  var = var.empty() ? std::format("#{:04}", variable) : var;
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Input Number" + colon.data() +
         var + ", " + std::to_string(digits) + (digits > 1 ? " digits" : " digit") + ColorFormatter::popColor();
}
