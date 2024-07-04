#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LabelCommand : IEventCommand {
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  std::string label;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};