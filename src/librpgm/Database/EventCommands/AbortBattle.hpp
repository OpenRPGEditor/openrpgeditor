#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct AbortBattleCommand final : IEventCommand {
  AbortBattleCommand() = default;
  explicit AbortBattleCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~AbortBattleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Abort_Battle; }
};