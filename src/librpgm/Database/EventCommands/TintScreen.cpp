#include "Database/EventCommands/TintScreen.hpp"

TintScreenCommand::TintScreenCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  auto colorValues = parameters[0];
  colorValues[0].get_to(color.r);
  colorValues[1].get_to(color.g);
  colorValues[2].get_to(color.b);
  colorValues[3].get_to(color.gray);
  parameters[1].get_to(duration);
  parameters[2].get_to(waitForCompletion);
}

std::string TintScreenCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Tint Screen" + colon.data() +
         std::format("({},{},{},{}), {} frames", color.r, color.g, color.b, color.gray, duration) +
         ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
