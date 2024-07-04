#include "Database/EventCommands/RotatePicture.hpp"

std::string RotatePictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Rotate Picture" + colon.data() +
         "#" + std::to_string(picture) + ", " + std::to_string(rotation) + ColorFormatter::popColor();
}
