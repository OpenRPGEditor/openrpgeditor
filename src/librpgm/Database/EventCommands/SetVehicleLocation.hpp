#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetVehicleLocationCommand final : IEventCommand {
  SetVehicleLocationCommand() = default;
  explicit SetVehicleLocationCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~SetVehicleLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Vehicle_Location; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  VehicleType vehicle;
  TransferMode mode;
  int mapId;
  int x;
  int y;
};