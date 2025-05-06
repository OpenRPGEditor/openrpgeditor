#include "Database/EventCommands/ChangeActorImage.hpp"
#include "Database/Database.hpp"

ChangeActorImageCommand::ChangeActorImageCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(actor);
  parameters.at(1).get_to(charPicture);
  parameters.at(2).get_to(charIndex);
  parameters.at(3).get_to(facePicture);
  parameters.at(4).get_to(faceIndex);
  parameters.at(5).get_to(battlerPicture);
}

void ChangeActorImageCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(actor);
  out.push_back(charPicture);
  out.push_back(charIndex);
  out.push_back(facePicture);
  out.push_back(faceIndex);
  out.push_back(battlerPicture);
}

std::string ChangeActorImageCommand::stringRep(const Database& db, const bool colored) const {
  std::string charString = db.imageText(charPicture, charIndex);
  std::string faceString = db.imageText(facePicture, faceIndex);
  std::string battlerString = db.imageText(battlerPicture);

  const auto actName = db.actorNameOrId(actor);
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Change Actor Images") + colon.data() + actName + ", " + charString + ", " + faceString + ", " +
         battlerString + ColorFormatter::popColor(colored);
}
