#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangePlayerFollowersCommand final : IEventCommand {
  ChangePlayerFollowersCommand() = default;
  explicit ChangePlayerFollowersCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(followersEnabled);
  }
  ~ChangePlayerFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Player_Followers; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ValueControl followersEnabled;
};
