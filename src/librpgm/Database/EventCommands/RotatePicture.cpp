#include "Database/EventCommands/RotatePicture.hpp"

RotatePictureCommand::RotatePictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent, parameters) {
  parameters.at(0).get_to(picture);
  parameters.at(1).get_to(rotation);
}

void RotatePictureCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(picture);
  out.push_back(rotation);
}

std::string RotatePictureCommand::stringRep(const Database& db) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Rotate Picture" + colon.data() + "#" + std::to_string(picture) + ", " + std::to_string(rotation) +
         ColorFormatter::popColor();
}
