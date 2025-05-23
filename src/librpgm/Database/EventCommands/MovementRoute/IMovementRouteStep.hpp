#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct IMovementRouteStep : IEventCommand {
  IMovementRouteStep() = default;
  explicit IMovementRouteStep(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  [[nodiscard]] EventCode code() const override = 0;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override { return symbol(code()) + DecodeEnumName(code()); }
};