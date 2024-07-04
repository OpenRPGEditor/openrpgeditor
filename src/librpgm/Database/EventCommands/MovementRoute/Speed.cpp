#include "Database/EventCommands/MovementRoute/Speed.hpp"

std::string MovementSpeedCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + DecodeEnumName(code()) + colon.data() + std::to_string(speed);
}
