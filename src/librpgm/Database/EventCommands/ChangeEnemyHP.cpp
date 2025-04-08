#include "Database/EventCommands/ChangeEnemyHP.hpp"
#include "Database/Database.hpp"

ChangeEnemyHPCommand::ChangeEnemyHPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(enemy);
  parameters.at(1).get_to(enemyOp);
  parameters.at(2).get_to(quantitySource);
  parameters.at(3).get_to(quantity);
  parameters.at(4).get_to(allowKnockOut);
}

void ChangeEnemyHPCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(enemy);
  out.push_back(enemyOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
  out.push_back(allowKnockOut);
}

std::string ChangeEnemyHPCommand::stringRep(const Database& db, const bool colored) const {
  std::string enemyStr;
  if (enemy < 0) {
    enemyStr = tr("Entire Troop") + ", ";
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
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Allow Knockout")) + ColorFormatter::popColor(colored);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Enemy HP") + colon.data() + enemyStr + DecodeEnumName(enemyOp) + " " + quantityStr +
         ColorFormatter::popColor(colored) + suffix;
}
