#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct IMovementRouteStep : IEventCommand {
  IMovementRouteStep() = default;
  explicit IMovementRouteStep(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  [[nodiscard]] EventCode code() const override = 0;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return symbol(code()) + DecodeEnumName(code());
  }
};