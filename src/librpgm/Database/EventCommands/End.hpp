#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EndCommand final : IEventCommand {
  EndCommand() = default;
  explicit EndCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
};
