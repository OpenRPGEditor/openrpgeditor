#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeVehicleImageCommand final : IEventCommand {
  ChangeVehicleImageCommand() = default;
  explicit ChangeVehicleImageCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeVehicleImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vehicle_Image; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  VehicleType vehicle;
  std::string picture;
  int pictureIndex;
};
