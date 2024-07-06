#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeBattlebackCommand final : IEventCommand {
  ChangeBattlebackCommand() = default;
  explicit ChangeBattlebackCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeBattlebackCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_Back; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  std::string battleBack1Name;
  std::string battleBack2Name;
};