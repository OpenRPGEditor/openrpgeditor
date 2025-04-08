#include "Database/EventCommands/GetLocationInfo.hpp"
#include "Database/Database.hpp"

GetLocationInfoCommand::GetLocationInfoCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(variable);
  parameters.at(1).get_to(type);
  parameters.at(2).get_to(source);
  parameters.at(3).get_to(x);
  parameters.at(4).get_to(y);
}

void GetLocationInfoCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(variable);
  out.push_back(type);
  out.push_back(source);
  out.push_back(x);
  out.push_back(y);
}

std::string GetLocationInfoCommand::stringRep(const Database& db, const bool colored) const {
  std::string var = db.system.variable(variable);
  var = var.empty() ? std::format("#{:04}", variable) : var;
  if (source == LocationSource::Designation_with_variables) {
    auto varX = db.system.variable(x);
    auto varY = db.system.variable(y);
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Get Location Info") + colon.data() + var + ", " + DecodeEnumName(type) +
           std::format(", ({{{}}},{{{}}})", varX, varY) + ColorFormatter::popColor(colored);
  }

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Get Location Info") + colon.data() + var + ", " + DecodeEnumName(type) +
         std::format(", ({},{})", x, y) + ColorFormatter::popColor(colored);
}
