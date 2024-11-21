#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"

#include <format>

struct PlaySECommand final : IEventCommand {
  PlaySECommand() = default;
  explicit PlaySECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  Audio audio;
};
