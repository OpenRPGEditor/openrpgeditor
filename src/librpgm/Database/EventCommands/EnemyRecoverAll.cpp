#include "Database/EventCommands/EnemyRecoverAll.hpp"

#include "Database/Database.hpp"

EnemyRecoverAllCommand::EnemyRecoverAllCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(troop);
}

void EnemyRecoverAllCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(troop); }

std::string EnemyRecoverAllCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Recover All" + colon.data() + (troop >= 0 ? "#" + std::to_string(troop + 1) : "Entire Troop") +
         ColorFormatter::popColor();
}
