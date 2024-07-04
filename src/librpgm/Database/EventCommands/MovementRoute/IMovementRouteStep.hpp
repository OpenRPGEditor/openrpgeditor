#pragma once

#include "Database/EventCommands/IEventCommand.hpp"

struct IMovementRouteStep : IEventCommand {
  static constexpr std::string_view whiteDiamond = "\u25c7";
  [[nodiscard]] virtual EventCode code() const = 0;
  [[nodiscard]] std::string stringRep(const Database& db) const { return whiteDiamond.data() + DecodeEnumName(code()); }
};