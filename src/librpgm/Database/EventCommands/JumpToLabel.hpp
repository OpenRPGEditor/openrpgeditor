#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct JumpToLabelCommand : IEventCommand {
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  std::string label;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};