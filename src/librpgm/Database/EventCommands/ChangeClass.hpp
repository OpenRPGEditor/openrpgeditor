#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeClassCommand final : IEventCommand {
  ChangeClassCommand() = default;
  explicit ChangeClassCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeClassCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Class; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actor;
  int classId;
  bool saveLevel;
};