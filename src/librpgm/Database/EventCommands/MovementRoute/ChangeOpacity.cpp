#include "Database/EventCommands/MovementRoute/ChangeOpacity.hpp"

std::string MovementChangeOpacityCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Opacity") + colon.data() + std::to_string(opacity);
}
