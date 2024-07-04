#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct IMovementRouteStep : IEventCommand {
  static constexpr std::string_view whiteDiamond = "\u25c7";
  [[nodiscard]] virtual EventCode code() const override = 0;
  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return whiteDiamond.data() + DecodeEnumName(code());
  }
};