#include "Database/EventCommands/MovePicture.hpp"

#include "Database/Database.hpp"

std::string MovePictureCommand::stringRep(const Database& db) const {
  if (pictureLocation == PictureDesignationSource::Designation_with_variables) {
    std::string varX = db.system.variable(x);
    std::string varY = db.system.variable(y);
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Move Picture" + colon.data() +
           std::format("#{}, {} ({{{}}},{{{}}}), ({}%, {}%), {}, {}, {} frames", picture, DecodeEnumName(origin), varX,
                       varY, width, height, opacity, DecodeEnumName(blendMode), duration) +
           ColorFormatter::popColor() +
           (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                      : "");
  }
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Move Picture" + colon.data() +
         std::format("#{}, {} ({},{}), ({}%, {}%), {}, {}, {} frames", picture, DecodeEnumName(origin), x, y, width,
                     height, opacity, DecodeEnumName(blendMode), duration) +
         ColorFormatter::popColor() +
         (waitForCompletion == true ? ColorFormatter::getColor(Color::Gray) + " (Wait)" + ColorFormatter::popColor()
                                    : "");
}
