#include "Database/EventCommands/ErasePicture.hpp"

std::string ErasePictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Erase Picture" + colon.data() +
         std::to_string(picture) + ColorFormatter::popColor();
}
