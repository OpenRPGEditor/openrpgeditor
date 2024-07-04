#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeMenuAccessCommand : IEventCommand {
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};