#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeProfileCommand final : IEventCommand {
  ChangeProfileCommand() = default;
  explicit ChangeProfileCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeProfileCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Profile; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeProfileCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Actor) {
      return actor == targetId;
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
  int actor = 1;
  std::string profile;
};
