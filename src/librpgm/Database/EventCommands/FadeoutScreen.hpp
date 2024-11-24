#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutScreenCommand final : IEventCommand {
  FadeoutScreenCommand() = default;
  explicit FadeoutScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~FadeoutScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_Screen; }
};
