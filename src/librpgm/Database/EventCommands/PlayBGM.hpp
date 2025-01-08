#pragma once
#include "Database/Audio.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayBGMCommand final : IEventCommand {
  PlayBGMCommand() = default;
  explicit PlayBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlayBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGM; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<PlayBGMCommand>(*this); }

  Audio audio;
};
