#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include "Database/Audio.hpp"
#include <format>

struct ChangeBattleBGMCommand final : IEventCommand {
  ChangeBattleBGMCommand() = default;
  explicit ChangeBattleBGMCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(bgm);
  }
  ~ChangeBattleBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_BGM; }
  Audio bgm;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};