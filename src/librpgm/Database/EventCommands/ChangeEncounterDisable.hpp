#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeEncounterDisableCommand final : IEventCommand {
  ChangeEncounterDisableCommand() = default;
  explicit ChangeEncounterDisableCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(access);
  }
  ~ChangeEncounterDisableCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Encounter_Disable; }
  AccessMode access;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
