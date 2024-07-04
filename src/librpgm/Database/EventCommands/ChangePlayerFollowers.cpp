#include "Database/EventCommands/ChangePlayerFollowers.hpp"

std::string ChangePlayerFollowersCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Player Followers" +
         colon.data() + DecodeEnumName(followersEnabled) + ColorFormatter::popColor();
}
