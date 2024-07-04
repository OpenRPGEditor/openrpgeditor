#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct SetEventLocationCommand : IEventCommand {
  ~SetEventLocationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Set_Event_Location; }
  TransferMode mode;
  int event;
  int x;
  int y;
  Direction direction;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
