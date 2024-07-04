#include "Database/EventCommands/MovementRoute/Wait.hpp"

std::string MovementWaitCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Wait") + colon.data() + std::to_string(duration) + " frames";
}
