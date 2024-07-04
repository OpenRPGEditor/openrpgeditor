#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ResumeBGMCommand : IEventCommand {
  ~ResumeBGMCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Resume_BGM; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
