#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowAnimationCommand final : IEventCommand {
  ShowAnimationCommand() = default;
  explicit ShowAnimationCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShowAnimationCommand>(*this); }
  bool hasReference(int targetId, SearchType type) override {
    if (type == SearchType::Animation) {
      return animation == targetId;
    }
    return false;
  };
  bool setReference(int targetId, int newId, SearchType type) override {
    if (hasReference(targetId, type)) {
      animation = newId;
      return true;
    }
    return false;
  }
  int character = -1;
  int animation = 1;
  bool waitForCompletion = false;
};
