#include "Database/EventCommands/RecoverAll.hpp"
#include "Database/Database.hpp"

RecoverAllCommand::RecoverAllCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(comparison);
  parameters.at(1).get_to(value);
}

void RecoverAllCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(comparison);
  out.push_back(value);
}

std::string RecoverAllCommand::stringRep(const Database& db) const {
  const std::string source = comparison == ActorComparisonSource::Fixed
                                 ? value == 0 ? "Entire Party" : db.actorNameOrId(value)
                                 : "{" + db.system.variable(value) + "}";
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Recover All" + colon.data() +
         source + ColorFormatter::popColor();
}
