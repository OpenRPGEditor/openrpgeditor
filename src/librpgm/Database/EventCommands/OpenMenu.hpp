#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct OpenMenuCommand final : IEventCommand {
  OpenMenuCommand() = default;
  explicit OpenMenuCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~OpenMenuCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Menu_Screen; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<OpenMenuCommand>(*this); }
};
