#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeEquipmentCommand final : IEventCommand {
  ChangeEquipmentCommand() = default;
  explicit ChangeEquipmentCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeEquipmentCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Equipment; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int actorId;
  int equipType;
  int equipment;
};
