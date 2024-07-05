#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ScrollMapCommand final : IEventCommand {
  ScrollMapCommand() = default;
  explicit ScrollMapCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ScrollMapCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Scroll_Map; }
  Direction direction;
  int distance;
  MovementSpeed speed;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};