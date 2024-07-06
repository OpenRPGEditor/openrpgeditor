#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowAnimationCommand final : IEventCommand {
  ShowAnimationCommand() = default;
  explicit ShowAnimationCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int character = -1;
  int animation = 1;
  bool waitForCompletion = false;
};
