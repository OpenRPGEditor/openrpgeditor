#include "Database/EventCommands/TintPicture.hpp"

std::string TintPictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Tint Picture" + colon.data() +
         std::format("#{}, ({},{},{},{}), {} frames", picture, colors.r, colors.g, colors.b, colors.gray, duration) +
         ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
