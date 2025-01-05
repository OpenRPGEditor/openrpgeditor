#include "Database/EventCommands/FlashScreen.hpp"

FlashScreenCommand::FlashScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  auto colorValue = parameters.at(0);

  colorValue[0].get_to(color.r);
  colorValue[1].get_to(color.g);
  colorValue[2].get_to(color.b);
  colorValue[3].get_to(color.intensity);

  parameters.at(1).get_to(duration);
  parameters.at(2).get_to(waitForCompletion);
}

void FlashScreenCommand::serializeParameters(nlohmann::ordered_json& out) const {
  auto colorValue = nlohmann::ordered_json();

  colorValue.push_back(color.r);
  colorValue.push_back(color.g);
  colorValue.push_back(color.b);
  colorValue.push_back(color.intensity);

  out.push_back(colorValue);
  out.push_back(duration);
  out.push_back(waitForCompletion);
}

std::string FlashScreenCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Flash Screen" + colon.data() +
         std::format("({},{},{},{}), {} frames", color.r, color.g, color.b, color.intensity, duration) + ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(FormatColor::Gray) + " (Wait)" + ColorFormatter::popColor() : "");
}
