#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeProfileCommand final : IEventCommand {
  ChangeProfileCommand() = default;
  explicit ChangeProfileCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeProfileCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Profile; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actor;
  std::string profile;
};
