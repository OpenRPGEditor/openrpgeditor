#include "Database/EventCommands/ChangeHP.hpp"

#include "Database/Database.hpp"

ChangeHPCommand::ChangeHPCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(comparison);
  parameters.at(1).get_to(value);
  parameters.at(2).get_to(quantityOp);
  parameters.at(3).get_to(quantitySource);
  parameters.at(4).get_to(quantity);
  parameters.at(5).get_to(allowKnockout);
}

void ChangeHPCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(comparison);
  out.push_back(value);
  out.push_back(quantityOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
  out.push_back(allowKnockout);
}


std::string ChangeHPCommand::stringRep(const Database& db) const {
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
  if (allowKnockout) {
    suffix =
        ColorFormatter::getColor(Color::Gray) + " " + db.parentheses("Allow Knockout") + ColorFormatter::popColor();
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change HP" + colon.data() +
         actName + ", " + DecodeEnumName(quantityOp) + " " + quantityStr + ColorFormatter::popColor() + suffix;
}
