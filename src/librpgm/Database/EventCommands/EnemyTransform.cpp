#include "Database/EventCommands/EnemyTransform.hpp"
#include "Database/Database.hpp"

EnemyTransformCommand::EnemyTransformCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(troopMember);
  parameters.at(1).get_to(enemy);
}

void EnemyTransformCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(troopMember);
  out.push_back(enemy);
}

std::string EnemyTransformCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Enemy Transform") + colon.data() + " #" + std::to_string(troopMember + 1) + ", " +
         db.enemyNameOrId(enemy) + ColorFormatter::popColor(colored);
}
