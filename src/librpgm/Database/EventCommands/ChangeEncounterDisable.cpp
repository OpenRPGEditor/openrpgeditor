#include "Database/EventCommands/ChangeEncounterDisable.hpp"

ChangeEncounterDisableCommand::ChangeEncounterDisableCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(access);
}

void ChangeEncounterDisableCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(access); }

std::string ChangeEncounterDisableCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Encounter") + colon.data() + DecodeEnumName(access) + ColorFormatter::popColor(colored);
}
