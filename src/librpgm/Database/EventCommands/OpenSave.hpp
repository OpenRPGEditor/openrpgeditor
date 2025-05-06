#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct OpenSaveCommand final : IEventCommand {
  OpenSaveCommand() = default;
  explicit OpenSaveCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~OpenSaveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Save_Screen; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<OpenSaveCommand>(*this); }
};
