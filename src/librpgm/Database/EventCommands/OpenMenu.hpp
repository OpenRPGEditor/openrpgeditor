#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct OpenMenuCommand final : IEventCommand {
  ~OpenMenuCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Menu_Screen; }
};
