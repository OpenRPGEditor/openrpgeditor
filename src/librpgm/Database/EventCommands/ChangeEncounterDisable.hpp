#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeEncounterDisableCommand final : IEventCommand {
  ChangeEncounterDisableCommand() = default;
  explicit ChangeEncounterDisableCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeEncounterDisableCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Encounter_Disable; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  AccessMode access = AccessMode::Disable;
};
