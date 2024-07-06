#include "Database/EventCommands/ChangeParameter.hpp"

#include "Database/Database.hpp"

ChangeParameterCommand::ChangeParameterCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(comparison);
  parameters.at(1).get_to(value);
  parameters.at(2).get_to(param);
  parameters.at(3).get_to(quantityOp);
  parameters.at(4).get_to(quantitySource);
  parameters.at(5).get_to(quantity);
}

void ChangeParameterCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(comparison);
  out.push_back(value);
  out.push_back(param);
  out.push_back(quantityOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
}

std::string ChangeParameterCommand::stringRep(const Database& db) const {
  std::string actorName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value <= 0) {
      actorName = "Entire Party";
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

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Parameter" +
         colon.data() + actorName + ", " + DecodeEnumName(param) + " " + DecodeEnumName(quantityOp) + " " +
         quantityStr + ColorFormatter::popColor();
}
