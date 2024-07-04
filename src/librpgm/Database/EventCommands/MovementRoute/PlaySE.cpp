#include "Database/EventCommands/MovementRoute/PlaySE.hpp"
#include "Database/Database.hpp"

std::string MovementPlaySECommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("SE") + colon.data() + db.audioText(se);
}
