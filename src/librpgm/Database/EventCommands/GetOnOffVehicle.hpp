#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GetOnOffVehicleCommand final : IEventCommand {
  GetOnOffVehicleCommand() = default;
  explicit GetOnOffVehicleCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~GetOnOffVehicleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_On_Off_Vehicle; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
