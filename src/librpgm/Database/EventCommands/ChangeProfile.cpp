#include "Database/EventCommands/ChangeProfile.hpp"
#include "Database/Database.hpp"

ChangeProfileCommand::ChangeProfileCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(actor);
  parameters.at(1).get_to(profile);
}

void ChangeProfileCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(actor);
  out.push_back(profile);
}

std::string ChangeProfileCommand::stringRep(const Database& db, const bool colored) const {
  const auto actName = Database::instance()->actorNameOrId(actor);
  auto tmpProfile = profile;
  ReplaceStr(tmpProfile, "\n", " ");
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Profile") + colon.data() + actName + ", " + tmpProfile +
         ColorFormatter::popColor(colored);
}
