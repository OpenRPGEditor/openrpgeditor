#include "Database/EventCommands/ShowBalloonIcon.hpp"

#include "Database/Database.hpp"

ShowBalloonIconCommand::ShowBalloonIconCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(id);
  parameters[1].get_to(index);
  parameters[2].get_to(waitForCompletion);
}

void ShowBalloonIconCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(id);
  out.push_back(index);
  out.push_back(waitForCompletion);
}

std::string ShowBalloonIconCommand::stringRep(const Database& db) const {
  const auto evName = db.eventNameOrId(id);
  const auto suffix =
      waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor() : "";
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Balloon Icon" +
         colon.data() + evName + DecodeEnumName(index) + ColorFormatter::popColor() + suffix;
}
