#pragma once
#include "Database/Audio.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayMECommand final : IEventCommand {
  PlayMECommand() = default;
  explicit PlayMECommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlayMECommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_ME; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<PlayMECommand>(*this); }
  bool hasStringReference(const std::string& text, SearchType type) override {
    if (type == SearchType::Audio) {
      return text.contains(audio.name());
    }
    return false;
  };
  Audio audio;
};
