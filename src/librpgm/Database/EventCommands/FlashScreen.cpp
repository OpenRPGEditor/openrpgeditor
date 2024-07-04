#include "Database/EventCommands/FlashScreen.hpp"

std::string FlashScreenCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Flash Screen" + colon.data() +
         std::format("({},{},{},{}), {} frames", colors.r, colors.g, colors.b, colors.intensity, duration) +
         ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
