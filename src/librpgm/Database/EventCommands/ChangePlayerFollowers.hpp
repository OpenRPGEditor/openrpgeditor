#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangePlayerFollowersCommand : IEventCommand {
  ~ChangePlayerFollowersCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Player_Followers; }

  ValueControl followersEnabled;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
