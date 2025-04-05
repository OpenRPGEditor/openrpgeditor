#include "Database/EventCommands/ScrollMap.hpp"

ScrollMapCommand::ScrollMapCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(direction);
  parameters.at(1).get_to(distance);
  parameters.at(2).get_to(speed);
}

void ScrollMapCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(direction);
  out.push_back(distance);
  out.push_back(speed);
}

std::string ScrollMapCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) +
         std::format("Scroll Map{}{}, {}, {}", colon.data(), DecodeEnumName(direction), distance, DecodeEnumName(speed)) + ColorFormatter::popColor();
}
