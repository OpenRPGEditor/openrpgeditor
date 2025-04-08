#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangePlayerFollowersCommand final : IEventCommand {
  ChangePlayerFollowersCommand() = default;
  explicit ChangePlayerFollowersCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangePlayerFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Player_Followers; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangePlayerFollowersCommand>(*this); }

  ValueControl followersEnabled = ValueControl::ON;
};
