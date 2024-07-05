#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct PlayBGSCommand final : IEventCommand {
  PlayBGSCommand() = default;
  explicit PlayBGSCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~PlayBGSCommand() override = default;
  Audio audio;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
