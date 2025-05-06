#include "Database/EventCommands/Wait.hpp"

#include "Database/Database.hpp"

WaitCommand::WaitCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(duration);
}

void WaitCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(duration); }

std::string WaitCommand::stringRep(const Database& db, const bool colored) const {
  auto ret = indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Wait") + colon.data() + db.framesText(duration) + ColorFormatter::popColor(colored);
  return ret;
}
