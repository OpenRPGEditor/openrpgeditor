#include "Database/EventCommands/ShowPicture.hpp"

ShowPictureCommand::ShowPictureCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(number);
  parameters[1].get_to(imageName);
  parameters[2].get_to(origin);
  parameters[3].get_to(type);
  parameters[4].get_to(value1);
  parameters[5].get_to(value2);
  parameters[6].get_to(zoomX);
  parameters[7].get_to(zoomY);
  parameters[8].get_to(opacityValue);
  parameters[9].get_to(blendMode);
}

std::string ShowPictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Picture" + colon.data() +
         std::format("#{}, {}, {} ({},{}), ({}%, {}%), {}, {}", number, imageName.empty() ? "None" : imageName,
                     DecodeEnumName(origin), DecodeEnumName(type), value1, value2, zoomX, zoomY, opacityValue,
                     DecodeEnumName(blendMode)) +
         ColorFormatter::popColor();
}
