#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeTilesetCommand final : IEventCommand {
  ~ChangeTilesetCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Tileset; }
  int tileset;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};