#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeoutScreenCommand final : IEventCommand {
  FadeoutScreenCommand() = default;
  explicit FadeoutScreenCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~FadeoutScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadeout_Screen; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<FadeoutScreenCommand>(*this); }
};
