#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayBGSCommand final : IEventCommand {
  PlayBGSCommand() = default;
  explicit PlayBGSCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~PlayBGSCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  Audio audio;
};
