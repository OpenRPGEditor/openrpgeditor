#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"

MovementChangeImageCommand::MovementChangeImageCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(image);
  parameters.at(1).get_to(character);
}

void MovementChangeImageCommand::serializeParameters(nlohmann::json& out) const {
  out.push_back(image);
  out.push_back(character);
}

std::string MovementChangeImageCommand::stringRep(const Database& db) const {
  return whiteDiamond.data() + std::string("Image") + colon.data() + db.imageText(image) + " " +
         db.parentheses(std::to_string(character));
}
