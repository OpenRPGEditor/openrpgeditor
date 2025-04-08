#include "Database/EventCommands/ChangePlayerFollowers.hpp"

ChangePlayerFollowersCommand::ChangePlayerFollowersCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(followersEnabled);
}

void ChangePlayerFollowersCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(followersEnabled); }

std::string ChangePlayerFollowersCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Player Followers") + colon.data() + DecodeEnumName(followersEnabled) +
         ColorFormatter::popColor(colored);
}
