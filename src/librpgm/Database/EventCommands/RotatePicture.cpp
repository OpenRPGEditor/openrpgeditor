#include "Database/EventCommands/RotatePicture.hpp"

RotatePictureCommand::RotatePictureCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(picture);
  parameters[1].get_to(rotation);
}

std::string RotatePictureCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Rotate Picture" + colon.data() +
         "#" + std::to_string(picture) + ", " + std::to_string(rotation) + ColorFormatter::popColor();
}
