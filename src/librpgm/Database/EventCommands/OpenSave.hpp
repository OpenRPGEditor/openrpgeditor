#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct OpenSaveCommand final : IEventCommand {
  OpenSaveCommand() = default;
  explicit OpenSaveCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IEventCommand(indent, parameters) {}
  ~OpenSaveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Open_Save_Screen; }
};
