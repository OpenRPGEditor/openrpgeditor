#pragma once
#include "IEventCommand.hpp"
#include <format>

struct SaveBGMCommand final : IEventCommand {
  SaveBGMCommand() = default;
  explicit SaveBGMCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~SaveBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Save_BGM; }
};
