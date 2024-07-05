#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowAnimationCommand final : IEventCommand {
  ShowAnimationCommand() = default;
  explicit ShowAnimationCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(character);
    parameters[1].get_to(animation);
    parameters[2].get_to(waitForCompletion);
  }
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  int character;
  int animation;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
