#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct ChangeDefeatMECommand final : IEventCommand {
  ChangeDefeatMECommand() = default;
  explicit ChangeDefeatMECommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(me);
  }
  ~ChangeDefeatMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Defeat_ME; }
  Audio me;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};