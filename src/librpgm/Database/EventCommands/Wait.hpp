#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct WaitCommand final : IEventCommand {
  WaitCommand() = default;
  explicit WaitCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~WaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int duration = 1;
};
