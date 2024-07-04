#include "Database/EventCommands/ShowPicture.hpp"

std::string ShowPictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Show Picture" + colon.data() +
         std::format("#{}, {}, {} ({},{}), ({}%, {}%), {}, {}", number, imageName.empty() ? "None" : imageName,
                     DecodeEnumName(origin), DecodeEnumName(type), value1, value2, zoomX, zoomY, opacityValue,
                     DecodeEnumName(blendMode)) +
         ColorFormatter::popColor();
}
