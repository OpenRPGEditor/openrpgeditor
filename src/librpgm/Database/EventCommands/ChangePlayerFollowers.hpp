#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangePlayerFollowersCommand final : IEventCommand {
  ChangePlayerFollowersCommand() = default;
  explicit ChangePlayerFollowersCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangePlayerFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Player_Followers; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  ValueControl followersEnabled = ValueControl::ON;
};
