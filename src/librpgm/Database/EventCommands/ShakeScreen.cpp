#include "Database/EventCommands/ShakeScreen.hpp"

#include "Database/Database.hpp"

ShakeScreenCommand::ShakeScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(power);
  parameters.at(1).get_to(speed);
  parameters.at(2).get_to(duration);
  parameters.at(3).get_to(waitForCompletion);
}

void ShakeScreenCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(power);
  out.push_back(speed);
  out.push_back(duration);
  out.push_back(waitForCompletion);
}

std::string ShakeScreenCommand::stringRep(const Database& db, const bool colored) const {
  std::string suffix;
  if (waitForCompletion) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Wait")) + ColorFormatter::popColor(colored);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Shake Screen") + colon.data() +
         std::format("{}, {}, {}", power, speed, db.framesText(duration)) + ColorFormatter::popColor(colored) + suffix;
}
