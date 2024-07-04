#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TransferPlayerCommand : IEventCommand {
  ~TransferPlayerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Transfer_Player; }
  TransferMode mode;
  int mapId;
  int x;
  int y;
  Direction direction;
  Fade fade;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
