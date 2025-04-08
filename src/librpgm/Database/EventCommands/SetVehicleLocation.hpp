#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetVehicleLocationCommand final : IEventCommand {
  SetVehicleLocationCommand() = default;
  explicit SetVehicleLocationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~SetVehicleLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Vehicle_Location; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<SetVehicleLocationCommand>(*this); }

  VehicleType vehicle = VehicleType::Boat;
  TransferMode mode = TransferMode::Direct;
  int mapId = Database::instance()->mapInfos.currentMap() != nullptr ? Database::instance()->mapInfos.currentMap()->id() : 1;
  int x{0};
  int y{0};
};