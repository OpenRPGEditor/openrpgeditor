#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayBGMCommand final : IEventCommand {
  PlayBGMCommand() = default;
  explicit PlayBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlayBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGM; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  Audio audio;
};
