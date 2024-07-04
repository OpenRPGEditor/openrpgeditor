#include "Database/EventCommands/MovementRoute/Frequency.hpp"

std::string MovementFrequencyCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Frequency") + colon.data() + std::to_string(frequency);
}
