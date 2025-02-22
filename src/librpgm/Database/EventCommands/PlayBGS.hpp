#pragma once
#include "Database/Audio.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct PlayBGSCommand final : IEventCommand {
  PlayBGSCommand() = default;
  explicit PlayBGSCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~PlayBGSCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Play_BGS; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<PlayBGSCommand>(*this); }
  bool hasStringReference(const std::string& text, SearchType type) override {
    if (type == SearchType::Audio) {
      return text.contains(audio.name());
    }
    return false;
  };
  Audio audio;
};
