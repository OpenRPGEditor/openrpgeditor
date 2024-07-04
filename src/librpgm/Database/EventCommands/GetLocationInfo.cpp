#include "Database/EventCommands/GetLocationInfo.hpp"

#include "Database/Database.hpp"

std::string GetLocationInfoCommand::stringRep(const Database& db) const {
  std::string var = db.system.variable(variable);
  var = var.empty() ? std::format("#{:04}", variable) : var;
  if (source == LocationSource::Designation_with_variables) {
    auto varX = db.system.variable(x);
    auto varY = db.system.variable(y);
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Get Location Info" +
           colon.data() + var + ", " + DecodeEnumName(type) + std::format(", ({{{}}},{{{}}})", varX, varY) +
           ColorFormatter::popColor();
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Get Location Info" +
         colon.data() + var + ", " + DecodeEnumName(type) + std::format(", ({},{})", x, y) + ColorFormatter::popColor();
}
