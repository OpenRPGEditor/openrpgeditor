#include "Database/EventCommands/ShowAnimation.hpp"

#include "Database/Database.hpp"

ShowAnimationCommand::ShowAnimationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(character);
  parameters.at(1).get_to(animation);
  parameters.at(2).get_to(waitForCompletion);
}

void ShowAnimationCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(character);
  out.push_back(animation);
  out.push_back(waitForCompletion);
}

std::string ShowAnimationCommand::stringRep(const Database& db, const bool colored) const {
  const auto evName = db.eventNameOrId(character);
  const auto animName = db.animationNameOrId(animation);

  std::string suffix;
  if (waitForCompletion) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Wait")) + ColorFormatter::popColor(colored);
  }

  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Show Animation") + colon.data() + evName + ", " + animName +
         ColorFormatter::popColor(colored) + suffix;
}