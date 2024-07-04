#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeVehicleImageCommand : IEventCommand {
  ~ChangeVehicleImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vehicle_Image; }

  VehicleType vehicle;
  std::string picture;
  int pictureIndex;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
