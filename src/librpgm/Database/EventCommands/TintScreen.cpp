#include "Database/EventCommands/TintScreen.hpp"

#include "Database/Database.hpp"

TintScreenCommand::TintScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  auto colorValues = parameters.at(0);
  colorValues[0].get_to(color.r);
  colorValues[1].get_to(color.g);
  colorValues[2].get_to(color.b);
  colorValues[3].get_to(color.gray);
  parameters.at(1).get_to(duration);
  parameters.at(2).get_to(waitForCompletion);
}

void TintScreenCommand::serializeParameters(nlohmann::ordered_json& out) const {
  auto colorValues = nlohmann::json();
  colorValues.push_back(color.r);
  colorValues.push_back(color.g);
  colorValues.push_back(color.b);
  colorValues.push_back(color.gray);
  out.push_back(colorValues);
  out.push_back(duration);
  out.push_back(waitForCompletion);
}

std::string TintScreenCommand::stringRep(const Database& db, const bool colored) const {
  std::string suffix;
  if (waitForCompletion) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Wait")) + ColorFormatter::popColor(colored);
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Tint Screen") + colon.data() +
         std::format("({},{},{},{}), {}", color.r, color.g, color.b, color.gray, db.framesText(duration)) + ColorFormatter::popColor(colored) + suffix;
}
