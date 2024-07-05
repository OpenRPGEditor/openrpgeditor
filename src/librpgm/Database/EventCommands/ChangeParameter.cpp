#include "Database/EventCommands/ChangeParameter.hpp"

#include "Database/Database.hpp"

ChangeParameterCommand::ChangeParameterCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(comparison);
  parameters[1].get_to(value);
  parameters[2].get_to(param);
  parameters[3].get_to(quantityOp);
  parameters[4].get_to(quantitySource);
  parameters[5].get_to(quantity);
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
