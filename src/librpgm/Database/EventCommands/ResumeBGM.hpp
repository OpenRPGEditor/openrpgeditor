#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ResumeBGMCommand final : IEventCommand {
  ResumeBGMCommand() = default;
  explicit ResumeBGMCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~ResumeBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Resume_BGM; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ResumeBGMCommand>(*this); }
};
