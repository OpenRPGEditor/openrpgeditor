#include "Database/EventCommands/ChangeMapDisplayName.hpp"

ChangeMapNameDisplayCommand::ChangeMapNameDisplayCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(checkIfOn);
}

void ChangeMapNameDisplayCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(checkIfOn); }

std::string ChangeMapNameDisplayCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Map Name Display") + colon.data() + DecodeEnumName(checkIfOn) +
         ColorFormatter::popColor(colored);
}
