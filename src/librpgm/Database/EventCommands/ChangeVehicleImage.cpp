#include "Database/EventCommands/ChangeVehicleImage.hpp"

#include "Database/Database.hpp"

ChangeVehicleImageCommand::ChangeVehicleImageCommand(const std::optional<int>& indent, nlohmann::json& parameters)
: IEventCommand(indent, parameters) {
  parameters[0].get_to(vehicle);
  parameters[1].get_to(picture);
  parameters[2].get_to(pictureIndex);
}

std::string ChangeVehicleImageCommand::stringRep(const Database& db) const {
  return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code()) + "Change Vehicle Image" +
         colon.data() + DecodeEnumName(vehicle) + ", " + db.imageText(picture, pictureIndex) +
         ColorFormatter::popColor();
}
