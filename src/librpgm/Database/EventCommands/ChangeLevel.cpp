#include "Database/EventCommands/ChangeLevel.hpp"

#include "Database/Database.hpp"

ChangeLevelCommand::ChangeLevelCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(comparison);
  parameters.at(1).get_to(value);
  parameters.at(2).get_to(quantityOp);
  parameters.at(3).get_to(quantitySource);
  parameters.at(4).get_to(quantity);
  parameters.at(5).get_to(showLevelUp);
}

void ChangeLevelCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(comparison);
  out.push_back(value);
  out.push_back(quantityOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
  out.push_back(showLevelUp);
}

std::string ChangeLevelCommand::stringRep(const Database& db) const {
  std::string actName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value <= 0) {
      actName = "Entire Party";
    } else {
      actName = db.actorNameOrId(value);
    }
  } else {
    actName = std::format("{{{}}}", db.variableNameOrId(value));
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Constant) {
    quantityStr = std::to_string(quantity);
  } else {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  }

  std::string suffix;
  if (showLevelUp) {
    suffix = ColorFormatter::getColor(Color::Gray) + " (Show Level Up)" + ColorFormatter::popColor();
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Level" + colon.data() +
         actName + ", " + DecodeEnumName(quantityOp) + " " + quantityStr + ColorFormatter::popColor() + suffix;
}
