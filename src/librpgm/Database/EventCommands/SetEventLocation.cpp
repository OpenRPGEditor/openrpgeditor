#include "Database/EventCommands/SetEventLocation.hpp"
#include "Database/Database.hpp"

SetEventLocationCommand::SetEventLocationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
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

std::string SetEventLocationCommand::stringRep(const Database& db, const bool colored) const {
  const auto evName = db.eventNameOrId(event);
  auto dir = static_cast<Direction>(static_cast<int>(direction) * 2);
  const auto prefix = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Set Event Location") + colon.data() + evName;
  const auto suffix =
      direction == Direction::Retain ? "" : ColorFormatter::getColor(FormatColor::Gray, colored) + " (" + tr("Direction") + ": " + DecodeEnumName(dir) + ")" + ColorFormatter::popColor(colored);

  if (mode == TransferMode::Variable_Designation) {
    return prefix + std::format(", ({{{}}},{{{}}})", db.variableNameOrId(x), db.variableNameOrId(y)) + ColorFormatter::popColor() + suffix;
  }

  if (mode == TransferMode::Exchange_With_Another_Event) {
    return prefix + ", " + trNOOP("Exchange with") + " " + Database::instance()->eventNameOrId(x) + ColorFormatter::popColor() + suffix;
  }

  return prefix + std::format(", ({}, {})", x, y) + ColorFormatter::popColor() + suffix;
}
