#include "Database/EventCommands/EnemyTransform.hpp"
#include "Database/Database.hpp"

EnemyTransformCommand::EnemyTransformCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(enemy);
  parameters[1].get_to(transform);
}

std::string EnemyTransformCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Enemy Transform" + colon.data() +
         " #" + std::to_string(enemy + 1) + ", " + db.enemyNameOrId(transform) + ColorFormatter::popColor();
}
