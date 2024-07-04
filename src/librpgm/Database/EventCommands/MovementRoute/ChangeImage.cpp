#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"

std::string MovementChangeImageCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Image") + colon.data() + db.imageText(image) + " " +
         db.parentheses(std::to_string(character));
}
