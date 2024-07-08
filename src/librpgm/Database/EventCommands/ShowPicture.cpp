#include "Database/EventCommands/ShowPicture.hpp"

ShowPictureCommand::ShowPictureCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(number);
  parameters.at(1).get_to(imageName);
  parameters.at(2).get_to(origin);
  parameters.at(3).get_to(type);
  parameters.at(4).get_to(value1);
  parameters.at(5).get_to(value2);
  parameters.at(6).get_to(zoomX);
  parameters.at(7).get_to(zoomY);
  parameters.at(8).get_to(opacityValue);
  parameters.at(9).get_to(blendMode);
}

void ShowPictureCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(number);
  out.push_back(imageName);
  out.push_back(origin);
  out.push_back(type);
  out.push_back(value1);
  out.push_back(value2);
  out.push_back(zoomX);
  out.push_back(zoomY);
  out.push_back(opacityValue);
  out.push_back(blendMode);
}

std::string ShowPictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Picture" + colon.data() +
         std::format("#{}, {}, {} ({},{}), ({}%, {}%), {}, {}", number, imageName.empty() ? "None" : imageName,
                     DecodeEnumName(origin), value1, value2, zoomX, zoomY, opacityValue,
                     DecodeEnumName(blendMode)) +
         ColorFormatter::popColor();
}
