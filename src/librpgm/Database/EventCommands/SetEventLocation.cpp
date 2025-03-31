#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/Database.hpp"

SetEventLocationCommand::SetEventLocationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  parameters.at(0).get_to(event);
  parameters.at(1).get_to(mode);
  parameters.at(2).get_to(x); // Stores event designation ID
  parameters.at(3).get_to(y);
  parameters.at(4).get_to(direction);
}

void SetEventLocationCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(event);
  out.push_back(mode);
  out.push_back(x); // Stores event designation ID
  out.push_back(y);
  out.push_back(direction);
}

std::string SetEventLocationCommand::stringRep(const Database& db) const {
  const auto evName = db.eventNameOrId(event);
  const auto prefix = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Set Event Location" + colon.data() + (event > 0 ? evName : "This Event");
  const auto suffix = ColorFormatter::getColor(FormatColor::Gray) + " (Direction: " + DecodeEnumName(direction) + ")" + ColorFormatter::popColor();

  if (mode == TransferMode::Variable_Designation) {
    return prefix + std::format(", ({{{}}},{{{}}})", db.variableNameOrId(x), db.variableNameOrId(y)) + suffix;
  }

  if (mode == TransferMode::Exchange_With_Another_Event) {
    return prefix + ", Exchange with " + Database::instance()->eventNameOrId(x) + suffix;
  }

  return prefix + std::format(", ({}, {})", x, y) + suffix;
}
