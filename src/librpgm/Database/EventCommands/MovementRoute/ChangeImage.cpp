#include "Database/EventCommands/MovementRoute/ChangeImage.hpp"

MovementChangeImageCommand::MovementChangeImageCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IMovementRouteStep(indent, parameters) {
  parameters.at(0).get_to(image);
  parameters.at(1).get_to(character);
}

void MovementChangeImageCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(image);
  out.push_back(character);
}

std::string MovementChangeImageCommand::stringRep(const Database& db) const {
  return symbol(code()) + std::string("Image") + colon.data() + db.imageText(image) + " " +
         db.parentheses(std::to_string(character));
}
