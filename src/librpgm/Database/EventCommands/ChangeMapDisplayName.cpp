#include "Database/EventCommands/ChangeMapDisplayName.hpp"

ChangeMapNameDisplayCommand::ChangeMapNameDisplayCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(checkIfOn);
}

void ChangeMapNameDisplayCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(checkIfOn);
}

std::string ChangeMapNameDisplayCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Map Name Display" +
         colon.data() + DecodeEnumName(checkIfOn) + ColorFormatter::popColor();
}
