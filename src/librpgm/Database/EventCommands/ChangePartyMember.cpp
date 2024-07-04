#include "Database/EventCommands/ChangePartyMember.hpp"

#include "Database/Database.hpp"

std::string ChangePartyMemberCommand::stringRep(const Database& db) const {
  const auto actName = db.actorNameOrId(member);
  std::string suffix;
  if (initialize) {
    suffix = ColorFormatter::getColor(Color::Gray) + " (Initialize)" + ColorFormatter::popColor();
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Party Member" +
         colon.data() + DecodeEnumName(operation) + " " + actName + ColorFormatter::popColor() + suffix;
}
