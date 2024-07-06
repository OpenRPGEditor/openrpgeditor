#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RecoverAllCommand final : IEventCommand {
  RecoverAllCommand() = default;
  explicit RecoverAllCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~RecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Recover_All; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value = 1;

};

