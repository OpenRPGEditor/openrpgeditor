#include "Database/EventCommands/ChangeTP.hpp"

#include "Database/Database.hpp"

ChangeTPCommand::ChangeTPCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(comparison);
  parameters[1].get_to(value);
  parameters[2].get_to(quantityOp);
  parameters[3].get_to(quantitySource);
  parameters[4].get_to(quantity);
}

void ChangeTPCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(comparison);
  out.push_back(value);
  out.push_back(quantityOp);
  out.push_back(quantitySource);
  out.push_back(quantity);
}


std::string ChangeTPCommand::stringRep(const Database& db) const {
  std::string actorName;
  if (comparison == ActorComparisonSource::Fixed) {
    if (value <= 0) {
      actorName = "Entire Party";
    } else {
      actorName = db.actorNameOrId(value);
    }
  } else {
    actorName = db.variableNameOrId(value);
  }

  std::string quantityStr;
  if (quantitySource == QuantityChangeSource::Variable) {
    quantityStr = std::format("{{{}}}", db.variableNameOrId(quantity));
  } else {
    quantityStr = std::to_string(quantity);
  }

  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change TP" + colon.data() +
         actorName + ", " + DecodeEnumName(quantityOp) + " " + quantityStr + ColorFormatter::popColor();
}
