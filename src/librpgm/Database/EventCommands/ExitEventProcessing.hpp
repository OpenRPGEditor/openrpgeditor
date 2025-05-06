#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ExitEventProcessingCommand final : IEventCommand {
  ExitEventProcessingCommand() = default;
  explicit ExitEventProcessingCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~ExitEventProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Exit_Event_Processing; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ExitEventProcessingCommand>(*this); }
};