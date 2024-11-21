#include "Database/EventCommands/ChangeName.hpp"
#include "Database/Database.hpp"

ChangeNameCommand::ChangeNameCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(actor);
  parameters.at(1).get_to(name);
}

void ChangeNameCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(actor);
  out.push_back(name);
}

std::string ChangeNameCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Name" + colon.data() +
         db.actorNameOrId(actor) + ", " + name + ColorFormatter::popColor();
}
