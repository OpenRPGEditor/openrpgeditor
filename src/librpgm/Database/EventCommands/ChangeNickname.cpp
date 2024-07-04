#include "Database/EventCommands/ChangeNickname.hpp"

#include "Database/Database.hpp"

std::string ChangeNicknameCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Nickname" + colon.data() +
         db.actorNameOrId(actor) + ", " + nick + ColorFormatter::popColor();
}
