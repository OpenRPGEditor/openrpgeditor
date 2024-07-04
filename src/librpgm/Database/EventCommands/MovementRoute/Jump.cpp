#include "Database/EventCommands/MovementRoute/Jump.hpp"

std::string MovementJumpCommand::stringRep(const Database& db) const {
  return std::format("{}Jump{}{}, {}", whiteDiamond, colon, x, y);
}
