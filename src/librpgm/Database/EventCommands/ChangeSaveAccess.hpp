#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeSaveAccessCommand : IEventCommand {
  ~ChangeSaveAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Save_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
