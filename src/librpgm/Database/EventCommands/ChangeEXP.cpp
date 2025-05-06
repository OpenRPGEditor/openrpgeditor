#include "Database/EventCommands/ChangeEXP.hpp"

#include "Database/Database.hpp"

ChangeEXPCommand::ChangeEXPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(comparison);
  parameters.at(1).get_to(value);
  parameters.at(2).get_to(quantityOp);
  parameters.at(3).get_to(quantitySource);
  parameters.at(4).get_to(quantity);
  parameters.at(5).get_to(showLevelUp);
}

void ChangeEXPCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(comparison);
  out.push_back(value);
  out.push_back(quantityOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
  out.push_back(showLevelUp);
}

std::string ChangeEXPCommand::stringRep(const Database& db, const bool colored) const {

  std::string actorName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value <= 0) {
      actorName = trNOOP("Entire Party");
    } else {
      actorName = db.actorNameOrId(value);
    }
  } else {
    actorName = std::format("{{{}}}", db.variableNameOrId(value));
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Constant) {
    quantityStr = std::to_string(quantity);
  } else {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  }

  std::string suffix;
  if (showLevelUp) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Show Level Up")) + ColorFormatter::popColor(colored);
  }

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change EXP") + colon.data() + actorName + ", " + DecodeEnumName(quantityOp) + " " +
         quantityStr + ColorFormatter::popColor(colored) + suffix;
}
