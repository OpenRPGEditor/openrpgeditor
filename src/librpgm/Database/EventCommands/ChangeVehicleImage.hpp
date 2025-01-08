#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeVehicleImageCommand final : IEventCommand {
  ChangeVehicleImageCommand() = default;
  explicit ChangeVehicleImageCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeVehicleImageCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vehicle_Image; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeVehicleImageCommand>(*this); }

  VehicleType vehicle = VehicleType::Boat;
  std::string picture;
  int pictureIndex{0};
};
