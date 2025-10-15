#include "Database/EventCommands/MovePicture.hpp"
#include "Database/Database.hpp"

MovePictureCommand::MovePictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(picture);
  // param[1] is not used
  parameters.at(2).get_to(origin);
  parameters.at(3).get_to(pictureLocation);
  parameters.at(4).get_to(x);
  parameters.at(5).get_to(y);
  parameters.at(6).get_to(width);
  parameters.at(7).get_to(height);
  parameters.at(8).get_to(opacity);
  parameters.at(9).get_to(blendMode);
  parameters.at(10).get_to(duration);
  parameters.at(11).get_to(waitForCompletion);
}

void MovePictureCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(picture);
  out.push_back(0); // param[1] is not used
  out.push_back(origin);
  out.push_back(pictureLocation);
  out.push_back(x);
  out.push_back(y);
  out.push_back(width);
  out.push_back(height);
  out.push_back(opacity);
  out.push_back(blendMode);
  out.push_back(duration);
  out.push_back(waitForCompletion);
}

std::string MovePictureCommand::stringRep(const Database& db, const bool colored) const {
  std::string suffix;
  if (waitForCompletion) {
    suffix = ColorFormatter::getColor(FormatColor::Gray, colored) + " " + db.parentheses(trNOOP("Wait")) + ColorFormatter::popColor(colored);
  }
  if (pictureLocation == PictureDesignationSource::Designation_with_variables) {
    std::string varX = db.system->variable(x);
    std::string varY = db.system->variable(y);
    varX = varX.empty() ? std::format("#{:04}", x) : varX;
    varY = varY.empty() ? std::format("#{:04}", y) : varY;
    return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Move Picture") + colon.data() +
           std::format("#{}, {} ({{{}}},{{{}}}), ({}%, {}%), {}, {}, {}", picture, DecodeEnumName(origin), varX, varY, width, height, opacity, DecodeEnumName(blendMode), db.framesText(duration)) +
           ColorFormatter::popColor(colored) + suffix;
  }
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Move Picture") + colon.data() +
         std::format("#{}, {} ({},{}), ({}%, {}%), {}, {}, {}", picture, DecodeEnumName(origin), x, y, width, height, opacity, DecodeEnumName(blendMode), db.framesText(duration)) +
         ColorFormatter::popColor(colored) + suffix;
}
