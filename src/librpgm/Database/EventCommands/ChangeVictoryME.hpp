#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct ChangeVictoryMECommand final : IEventCommand {
  ChangeVictoryMECommand() = default;
  explicit ChangeVictoryMECommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(me);
  }
  ~ChangeVictoryMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Victory_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};