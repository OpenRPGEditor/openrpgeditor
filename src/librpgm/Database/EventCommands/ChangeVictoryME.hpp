#pragma once
#include "Database/Audio.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeVictoryMECommand final : IEventCommand {
  ChangeVictoryMECommand() = default;
  explicit ChangeVictoryMECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeVictoryMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Victory_ME; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeVictoryMECommand>(*this); }

  Audio me;
};