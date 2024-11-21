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

  int actor = 1;
  std::string profile;
};
