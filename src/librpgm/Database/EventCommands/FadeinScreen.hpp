#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeinScreenCommand final : IEventCommand {
  FadeinScreenCommand() = default;
  explicit FadeinScreenCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~FadeinScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadein_Screen; }
};
