#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GetOnOffVehicleCommand final : IEventCommand {
  GetOnOffVehicleCommand() = default;
  explicit GetOnOffVehicleCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IEventCommand(indent, parameters) {}
  ~GetOnOffVehicleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_on_fwsl_off_Vehicle; }
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<GetOnOffVehicleCommand>(*this); }
};
