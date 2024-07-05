#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct NameInputCommand final : IEventCommand {
  ~NameInputCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Name_Input_Processing; }

  int actorId;
  int maxChar;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
