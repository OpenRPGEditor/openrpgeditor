#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RecoverAllCommand final : IEventCommand {
  ~RecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Recover_All; }

  ActorComparisonSource comparison;
  int value;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

