#include "Database/EventCommands/ChangePartyMember.hpp"

#include "Database/Database.hpp"

ChangePartyMemberCommand::ChangePartyMemberCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(member);
  parameters.at(1).get_to(operation);
  parameters.at(2).get_to(initialize);
}

void ChangePartyMemberCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(member);
  out.push_back(operation);
  out.push_back(initialize);
}

std::string ChangePartyMemberCommand::stringRep(const Database& db, const bool colored) const {
  const auto actName = db.actorNameOrId(member);
  std::string suffix;
  if (initialize) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Initialize")) + ColorFormatter::popColor(colored);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Party Member") + colon.data() +
         (operation == PartyMemberOperation::_daa__del_Remove ? trNOOP("Remove") : trNOOP("Add")) + " " + actName + ColorFormatter::popColor(colored) + suffix;
}
