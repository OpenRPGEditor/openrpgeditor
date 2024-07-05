#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeClassCommand final : IEventCommand {
  ChangeClassCommand() = default;
  explicit ChangeClassCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeClassCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Class; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actor;
  int classId;
  bool saveLevel;
};