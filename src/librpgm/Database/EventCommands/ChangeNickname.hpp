#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeNicknameCommand final : IEventCommand {
  ChangeNicknameCommand() = default;
  explicit ChangeNicknameCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeNicknameCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Nickname; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeNicknameCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Actor) {
      return targetId == actor;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      if (type == SearchType::Actor) {
        actor = newId;
      }
      return true;
    }
    return false;
  }
  int actor;
  std::string nick;
};
