#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>
struct ChangeVehicleBGMCommand : IEventCommand {
  ~ChangeVehicleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  VehicleType vehicle;
  Audio me;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

