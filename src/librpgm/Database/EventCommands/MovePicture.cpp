#include "Database/EventCommands/MovePicture.hpp"
#include "Database/Database.hpp"

MovePictureCommand::MovePictureCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(picture);
  // param[1] is not used
  parameters[2].get_to(origin);
  parameters[3].get_to(pictureLocation);
  parameters[4].get_to(x);
  parameters[5].get_to(y);
  parameters[6].get_to(width);
  parameters[7].get_to(height);
  parameters[8].get_to(opacity);
  parameters[9].get_to(blendMode);
  parameters[10].get_to(duration);
  parameters[11].get_to(waitForCompletion);
}

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
