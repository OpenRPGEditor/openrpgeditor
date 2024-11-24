#pragma once
#include "Database/Audio.hpp"
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeBattleBGMCommand final : IEventCommand {
  ChangeBattleBGMCommand() = default;
  explicit ChangeBattleBGMCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeBattleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_BGM; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  Audio bgm;
};