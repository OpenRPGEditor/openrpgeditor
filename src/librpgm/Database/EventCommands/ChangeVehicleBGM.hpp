#pragma once
#include "Database/Audio.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>
struct ChangeVehicleBGMCommand final : IEventCommand {
  ChangeVehicleBGMCommand() = default;
  explicit ChangeVehicleBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeVehicleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeVehicleBGMCommand>(*this); }

  VehicleType vehicle = VehicleType::Boat;
  Audio bgm;
};
