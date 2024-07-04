#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct GetLocationInfoCommand : IEventCommand {
  ~GetLocationInfoCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Get_Location_Info; }
  int variable;
  TileType type;
  LocationSource source;
  int x;
  int y;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
