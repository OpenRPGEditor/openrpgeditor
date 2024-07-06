#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayMECommand final : IEventCommand {
  PlayMECommand() = default;
  explicit PlayMECommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~PlayMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_ME; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  Audio audio;
};
