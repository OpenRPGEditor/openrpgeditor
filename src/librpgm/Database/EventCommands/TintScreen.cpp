#include "Database/EventCommands/TintScreen.hpp"

std::string TintScreenCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Tint Screen" + colon.data() +
         std::format("({},{},{},{}), {} frames", colors.r, colors.g, colors.b, colors.gray, duration) +
         ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
