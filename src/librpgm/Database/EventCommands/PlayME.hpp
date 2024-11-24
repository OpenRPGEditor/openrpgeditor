#pragma once
#include "Database/Audio.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayMECommand final : IEventCommand {
  PlayMECommand() = default;
  explicit PlayMECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlayMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_ME; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  Audio audio;
};
