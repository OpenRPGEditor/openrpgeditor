#include "Database/EventCommands/ShowBalloonIcon.hpp"

#include "Database/Database.hpp"

ShowBalloonIconCommand::ShowBalloonIconCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(id);
  parameters.at(1).get_to(index);
  parameters.at(2).get_to(waitForCompletion);
}

void ShowBalloonIconCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(id);
  out.push_back(index);
  out.push_back(waitForCompletion);
}

std::string ShowBalloonIconCommand::stringRep(const Database& db, const bool colored) const {
  const auto evName = db.eventNameOrId(id);
  std::string suffix;
  if (waitForCompletion) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Wait")) + ColorFormatter::popColor(colored);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Show Balloon Icon") + colon.data() + evName + DecodeEnumName(index) +
         ColorFormatter::popColor(colored) + suffix;
}
