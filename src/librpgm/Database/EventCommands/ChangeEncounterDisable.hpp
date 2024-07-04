#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeEncounterDisableCommand : IEventCommand {
  ~ChangeEncounterDisableCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Encounter_Disable; }
  AccessMode access;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
