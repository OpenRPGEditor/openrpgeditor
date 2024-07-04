#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetVehicleLocationCommand : IEventCommand {
  ~SetVehicleLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Vehicle_Location; }
  VehicleType vehicle;
  TransferMode mode;
  int mapId;
  int x;
  int y;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};