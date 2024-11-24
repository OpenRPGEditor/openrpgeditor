#include "Database/EventCommands/EnemyTransform.hpp"
#include "Database/Database.hpp"

EnemyTransformCommand::EnemyTransformCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  parameters.at(0).get_to(enemy);
  parameters.at(1).get_to(transform);
}

void EnemyTransformCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(enemy);
  out.push_back(transform);
}

std::string EnemyTransformCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Transform" + colon.data() + " #" + std::to_string(enemy + 1) + ", " + db.enemyNameOrId(transform) +
         ColorFormatter::popColor();
}
