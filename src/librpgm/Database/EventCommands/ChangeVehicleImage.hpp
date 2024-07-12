#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeVehicleImageCommand final : IEventCommand {
  ChangeVehicleImageCommand() = default;
  explicit ChangeVehicleImageCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeVehicleImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vehicle_Image; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  VehicleType vehicle = VehicleType::Boat;
  std::string picture;
  int pictureIndex{0};
};
