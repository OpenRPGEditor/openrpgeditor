#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct WaitCommand final : IEventCommand {
  WaitCommand() = default;
  explicit WaitCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(duration);
  }
  ~WaitCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Wait; }
  void serializeParameters(nlohmann::json& out) override { out.push_back(duration); }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int duration;
};
