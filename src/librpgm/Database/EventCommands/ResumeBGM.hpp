#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ResumeBGMCommand final : IEventCommand {
  ResumeBGMCommand() = default;
  explicit ResumeBGMCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~ResumeBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Resume_BGM; }
};
