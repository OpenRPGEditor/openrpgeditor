#include "Database/EventCommands/ChangeEnemyHP.hpp"
#include "Database/Database.hpp"

ChangeEnemyHPCommand::ChangeEnemyHPCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(enemy);
  parameters[1].get_to(enemyOp);
  parameters[2].get_to(quantitySource);
  parameters[3].get_to(quantity);
  parameters[4].get_to(allowKnockOut);
}

std::string ChangeEnemyHPCommand::stringRep(const Database& db) const {
  std::string enemyStr;
  if (enemy < 0) {
    enemyStr = "Entire Troop, ";
  } else {
    enemyStr = std::format("#{}, ", enemy + 1);
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Variable) {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  } else {
    quantityStr = std::format("{}", quantity);
  }

  std::string suffix;
  if (allowKnockOut) {
    suffix =
        ColorFormatter::getColor(Color::Gray) + " " + db.parentheses("Allow Knockout") + ColorFormatter::popColor();
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Enemy HP" + colon.data() +
         enemyStr + DecodeEnumName(enemyOp) + " " + quantityStr + ColorFormatter::popColor() + suffix;
}
