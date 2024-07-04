#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ScrollMapCommand : IEventCommand {
  ~ScrollMapCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Scroll_Map; }
  Direction direction;
  int distance;
  MovementSpeed speed;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};