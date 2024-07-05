#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct OpenSaveCommand final : IEventCommand {
  ~OpenSaveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Save_Screen; }
};
