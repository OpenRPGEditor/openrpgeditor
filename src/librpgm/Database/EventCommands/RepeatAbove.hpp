#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct RepeatAboveCommand : IEventCommand {
  ~RepeatAboveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Repeat_Above; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
