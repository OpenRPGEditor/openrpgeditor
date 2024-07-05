#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EraseEventCommand final : IEventCommand {
  EraseEventCommand() = default;
  explicit EraseEventCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~EraseEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Erase_Event; }
};
