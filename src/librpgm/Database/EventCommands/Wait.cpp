#include "Database/EventCommands/Wait.hpp"

WaitCommand::WaitCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(duration);
}

void WaitCommand::serializeParameters(nlohmann::json& out) const { out.push_back(duration); }

std::string WaitCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) +
         "Wait : " + std::to_string(duration) + " frames" + ColorFormatter::popColor();
}
