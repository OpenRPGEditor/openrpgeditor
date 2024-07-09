#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Database.hpp"
#include <format>

struct SetVehicleLocationCommand final : IEventCommand {
  SetVehicleLocationCommand() = default;
  explicit SetVehicleLocationCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~SetVehicleLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Vehicle_Location; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  VehicleType vehicle = VehicleType::Boat;
  TransferMode mode = TransferMode::Direct;
  int mapId = Database::Instance->mapInfos.currentMap() != nullptr ? Database::Instance->mapInfos.currentMap()->id : 1;
  int x{0};
  int y{0};
};