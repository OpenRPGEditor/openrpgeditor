#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayMECommand final : IEventCommand {
  PlayMECommand() = default;
  explicit PlayMECommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~PlayMECommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_ME; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
