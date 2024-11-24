#pragma once
#include "Database/ColorFormatter.hpp"
#include "Database/EventCommands/IEventCommand.hpp"

struct EventDummy : IEventCommand {
  EventDummy() = default;
  explicit EventDummy(const std::optional<int>& _indent, const nlohmann::ordered_json& parameters) : IEventCommand(_indent, parameters) {}
  ~EventDummy() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Event_Dummy; }
  [[nodiscard]] std::string stringRep(const Database& db) const override { return indentText(indent) + symbol(code()); }
};