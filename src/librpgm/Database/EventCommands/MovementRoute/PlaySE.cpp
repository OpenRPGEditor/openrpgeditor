#include "Database/EventCommands/MovementRoute/PlaySE.hpp"
#include "Database/Database.hpp"

MovementPlaySECommand::MovementPlaySECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent) {
  parameters.at(0).get_to(se);
}

void MovementPlaySECommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(se); }

std::string MovementPlaySECommand::stringRep(const Database& db, const bool colored) const { return symbol(code()) + tr("SE") + colon.data() + db.audioText(se); }
