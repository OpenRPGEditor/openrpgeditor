#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeBattlebackCommand final : IEventCommand {
  ChangeBattlebackCommand() = default;
  explicit ChangeBattlebackCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeBattlebackCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Battle_Back; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeBattlebackCommand>(*this); }

  std::string battleBack1Name;
  std::string battleBack2Name;
};