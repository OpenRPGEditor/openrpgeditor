#include "Database/EventCommands/ChangePlayerFollowers.hpp"

ChangePlayerFollowersCommand::ChangePlayerFollowersCommand(const std::optional<int>& indent,
                                                           const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(followersEnabled);
}

void ChangePlayerFollowersCommand::serializeParameters(nlohmann::json& out) const { out.push_back(followersEnabled); }

std::string ChangePlayerFollowersCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Player Followers" +
         colon.data() + DecodeEnumName(followersEnabled) + ColorFormatter::popColor();
}
