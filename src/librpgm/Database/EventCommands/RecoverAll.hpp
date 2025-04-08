#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct RecoverAllCommand final : IEventCommand {
  RecoverAllCommand() = default;
  explicit RecoverAllCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~RecoverAllCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Recover_All; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<RecoverAllCommand>(*this); }

  ActorComparisonSource comparison = ActorComparisonSource::Fixed;
  int value = 1;
};
