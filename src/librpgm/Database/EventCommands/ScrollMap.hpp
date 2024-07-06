#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ScrollMapCommand final : IEventCommand {
  ScrollMapCommand() = default;
  explicit ScrollMapCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ScrollMapCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Scroll_Map; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  Direction direction;
  int distance;
  MovementSpeed speed;
};