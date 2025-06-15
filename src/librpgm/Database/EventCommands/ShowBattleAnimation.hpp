#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowBattleAnimationCommand final : IEventCommand {
  ShowBattleAnimationCommand() = default;
  explicit ShowBattleAnimationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ShowBattleAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Battle_Animation; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowBattleAnimationCommand>(*this); }
  bool hasReference(const int targetId, const SearchType type) override {
    if (type == SearchType::Animation) {
      return animation == targetId;
    }
    return false;
  };
  bool setReference(const int targetId, const int newId, const SearchType type) override {
    if (hasReference(targetId, type)) {
      animation = newId;
      return true;
    }
    return false;
  }
  int enemy = 0;
  int animation = 1;
  bool targetAllEnemies = false;

  int m_troopId = 0;
};
