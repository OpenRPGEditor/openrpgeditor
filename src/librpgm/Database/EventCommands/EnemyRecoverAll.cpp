#include "Database/EventCommands/EnemyRecoverAll.hpp"

#include "Database/Database.hpp"

EnemyRecoverAllCommand::EnemyRecoverAllCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(troop);
}

void EnemyRecoverAllCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(troop); }

std::string EnemyRecoverAllCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Enemy Recover All") + colon.data() + db.troopNameAndId(troop) +
         ColorFormatter::popColor(colored);
}
