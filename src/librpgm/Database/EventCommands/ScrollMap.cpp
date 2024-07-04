#include "Database/EventCommands/ScrollMap.hpp"

std::string ScrollMapCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) +
         std::format("Scroll Map{}{}, {}, {}", colon.data(), DecodeEnumName(direction), distance,
                     DecodeEnumName(speed)) +
         ColorFormatter::popColor();
}
