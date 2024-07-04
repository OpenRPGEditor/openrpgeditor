#include "Database/EventCommands/MovementRoute/SwitchOFF.hpp"
#include "Database/Database.hpp"

std::string MovementSwitchOFFCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + std::format("{:04}", id);
}
