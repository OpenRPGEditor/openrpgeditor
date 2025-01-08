#pragma once
#include "Database/Audio.hpp"
#include "Database/EventCommands/IEventCommand.hpp"

#include <format>

struct PlaySECommand final : IEventCommand {
  PlaySECommand() = default;
  explicit PlaySECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlaySECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_SE; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<PlaySECommand>(*this); }

  Audio audio;
};
