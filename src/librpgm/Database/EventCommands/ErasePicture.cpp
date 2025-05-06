#include "Database/EventCommands/ErasePicture.hpp"

ErasePictureCommand::ErasePictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
: IEventCommand(indent) {
  parameters.at(0).get_to(picture);
}

void ErasePictureCommand::serializeParameters(nlohmann::ordered_json& out) const { out.push_back(picture); }

std::string ErasePictureCommand::stringRep(const Database& db, const bool colored) const {
  return indentText(indent()) + symbol(code()) + ColorFormatter::getColorCode(code(), colored) + trNOOP("Erase Picture") + colon.data() + "#" + std::to_string(picture) +
         ColorFormatter::popColor(colored);
}
