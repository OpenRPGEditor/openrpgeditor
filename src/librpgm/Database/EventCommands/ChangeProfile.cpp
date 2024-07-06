#include "Database/EventCommands/ChangeProfile.hpp"
#include "Database/Database.hpp"

ChangeProfileCommand::ChangeProfileCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(actor);
  parameters[1].get_to(profile);
}

void ChangeProfileCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(actor);
  out.push_back(profile);
}

std::string ChangeProfileCommand::stringRep(const Database& db) const {
  const auto act = db.actors.actor(actor);
  const auto actName = act && !act->name.empty() ? act->name : std::format("#{:04}", actor);
  auto tmpProfile = profile;
  ReplaceStr(tmpProfile, "\n", " ");
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Profile" + colon.data() +
         actName + ", " + tmpProfile + ColorFormatter::popColor();
}
