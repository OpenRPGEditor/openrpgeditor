#include "Database/EventCommands/ScrollMap.hpp"

ScrollMapCommand::ScrollMapCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(direction);
  parameters[1].get_to(distance);
  parameters[2].get_to(speed);
}

std::string ScrollMapCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) +
         std::format("Scroll Map{}{}, {}, {}", colon.data(), DecodeEnumName(direction), distance,
                     DecodeEnumName(speed)) +
         ColorFormatter::popColor();
}
