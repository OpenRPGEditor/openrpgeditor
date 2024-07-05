#include "Database/EventCommands/FlashScreen.hpp"

FlashScreenCommand::FlashScreenCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  auto colorValue = parameters[0];

  colorValue[0].get_to(color.r);
  colorValue[1].get_to(color.g);
  colorValue[2].get_to(color.b);
  colorValue[3].get_to(color.intensity);

  parameters[1].get_to(duration);
  parameters[2].get_to(waitForCompletion);
}

std::string FlashScreenCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Flash Screen" + colon.data() +
         std::format("({},{},{},{}), {} frames", color.r, color.g, color.b, color.intensity, duration) +
         ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
