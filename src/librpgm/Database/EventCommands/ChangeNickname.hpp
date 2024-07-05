#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeNicknameCommand final : IEventCommand {
  ChangeNicknameCommand() = default;
  explicit ChangeNicknameCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeNicknameCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Nickname; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actor;
  std::string nick;
};
