#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct IMovementRouteStep : IEventCommand {
  IMovementRouteStep() = default;
  explicit IMovementRouteStep(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  static constexpr std::string_view whiteDiamond = "\u25c7";
  [[nodiscard]] EventCode code() const override = 0;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return whiteDiamond.data() + DecodeEnumName(code());
  }
};