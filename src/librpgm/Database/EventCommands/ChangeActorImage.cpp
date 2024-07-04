#include "Database/EventCommands/ChangeActorImage.hpp"

#include "Database/Database.hpp"

std::string ChangeActorImageCommand::stringRep(const Database& db) const {
  std::string charString = db.imageText(charPicture, charIndex);
  std::string faceString = db.imageText(facePicture, faceIndex);
  std::string battlerString = db.imageText(battlerPicture);

  const auto actName = db.actorNameOrId(actor);
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Actor Images" +
         colon.data() + actName + ", " + charString + ", " + faceString + ", " + battlerString +
         ColorFormatter::popColor();
}
