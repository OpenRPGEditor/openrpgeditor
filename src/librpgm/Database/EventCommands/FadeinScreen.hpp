#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FadeinScreenCommand final : IEventCommand {
  FadeinScreenCommand() = default;
  explicit FadeinScreenCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~FadeinScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Fadein_Screen; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<FadeinScreenCommand>(*this); }
};
