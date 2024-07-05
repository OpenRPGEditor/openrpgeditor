#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>
struct ChangeVehicleBGMCommand final : IEventCommand {
  ChangeVehicleBGMCommand() = default;
  explicit ChangeVehicleBGMCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(vehicle);
    parameters[1].get_to(me);
  }
  ~ChangeVehicleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  VehicleType vehicle;
  Audio me;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
