#include "Database/EventCommands/ChangeFormationAccess.hpp"

ChangeFormationAccessCommand::ChangeFormationAccessCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  parameters.at(0).get_to(access);
}

void ChangeFormationAccessCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(access); }

std::string ChangeFormationAccessCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Formation Access" + colon.data() + DecodeEnumName(access) + ColorFormatter::popColor();
}