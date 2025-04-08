#include "Database/EventCommands/UnhandledEvent.hpp"

void UnhandledEventCommand::serialize(nlohmann::ordered_json& out, [[maybe_unused]] const bool doIndent, [[maybe_unused]] const bool doParameters, [[maybe_unused]] const bool oldValues) const {
  out.push_back(data);
}

std::string UnhandledEventCommand::stringRep(const Database& db, const bool colored) const {
  return IEventCommand::stringRep(db) + ColorFormatter::getColor(FormatColor::Red, colored) + " UNHANDLED" + ColorFormatter::popColor(colored);
}
