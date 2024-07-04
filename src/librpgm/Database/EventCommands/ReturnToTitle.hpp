#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ReturnToTitleCommand : IEventCommand {
  ~ReturnToTitleCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Return_To_Title_Screen; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};