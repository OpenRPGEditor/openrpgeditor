#include "Database/EventCommands/ChangeVehicleImage.hpp"

#include "Database/Database.hpp"

ChangeVehicleImageCommand::ChangeVehicleImageCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {
  parameters.at(0).get_to(vehicle);
  parameters.at(1).get_to(picture);
  parameters.at(2).get_to(pictureIndex);
}

void ChangeVehicleImageCommand::serializeParameters(nlohmann::ordered_json& out) const {
  out.push_back(vehicle);
  out.push_back(picture);
  out.push_back(pictureIndex);
}

std::string ChangeVehicleImageCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Vehicle Image" + colon.data() + DecodeEnumName(vehicle) + ", " + db.imageText(picture, pictureIndex) +
         ColorFormatter::popColor();
}
