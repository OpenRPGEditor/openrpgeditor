#include "Database/EventCommands/MovementRoute/PlaySE.hpp"
#include "Database/Database.hpp"

MovementPlaySECommand::MovementPlaySECommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters[0].get_to(se);
}

void MovementPlaySECommand::serializeParameters(nlohmann::json& out) { out.push_back(se); }

std::string MovementPlaySECommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("SE") + colon.data() + db.audioText(se);
}
