#include "Database/EventCommands/Plugin.hpp"

PluginCommandMV::PluginCommandMV(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(command);
}

void PluginCommandMV::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(command); }

std::string PluginCommandMV::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Plugin Command" + colon.data() +
         command + ColorFormatter::popColor();
}
