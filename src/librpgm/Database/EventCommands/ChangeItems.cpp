#include "Database/EventCommands/ChangeItems.hpp"

#include "Database/Database.hpp"

ChangeItemsCommand::ChangeItemsCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  parameters.at(0).get_to(item);
  parameters.at(1).get_to(operation);
  parameters.at(2).get_to(operandSource);
  parameters.at(3).get_to(operand);
}

void ChangeItemsCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(item);
  out.push_back(operation);
  out.push_back(operandSource);
  out.push_back(operand);
}

std::string ChangeItemsCommand::stringRep(const Database& db) const {
  std::string itemName = db.itemNameOrId(item);
  std::string operandStr;
  if (operandSource == QuantityChangeSource::Constant) {
    operandStr = std::to_string(operand);
  } else {
    operandStr = std::format("{{{}}}", db.variableNameOrId(operand));
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Items" + colon.data() + itemName + " " + DecodeEnumName(operation) + " " + operandStr +
         ColorFormatter::popColor();
}
