#include "Database/EventCommands/ChangePartyMember.hpp"

#include "Database/Database.hpp"

ChangePartyMemberCommand::ChangePartyMemberCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(member);
  parameters.at(1).get_to(operation);
  parameters.at(2).get_to(initialize);
}

void ChangePartyMemberCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(member);
  out.push_back(operation);
  out.push_back(initialize);
}

std::string ChangePartyMemberCommand::stringRep(const Database& db) const {
  const auto actName = db.actorNameOrId(member);
  std::string suffix;
  if (initialize) {
    suffix = ColorFormatter::getColor(FormatColor::Gray) + " (Initialize)" + ColorFormatter::popColor();
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Party Member" + colon.data() +
         (operation == PartyMemberOperation::_daa__del_Remove ? "Remove " : "Add ") + actName + ColorFormatter::popColor() + suffix;
}
