#include "Database/EventCommands/MovementRoute/SwitchON.hpp"

std::string MovementSwitchONCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + std::format("{:04}", id);
}
