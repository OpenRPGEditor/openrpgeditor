#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>
struct ChangeVehicleBGMCommand final : IEventCommand {
  ChangeVehicleBGMCommand() = default;
  explicit ChangeVehicleBGMCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeVehicleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Vechicle_BGM; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  VehicleType vehicle;
  Audio bgm;
};
