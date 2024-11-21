#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ScrollMapCommand final : IEventCommand {
  ScrollMapCommand() = default;
  explicit ScrollMapCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ScrollMapCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Scroll_Map; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  Direction direction = Direction::Down;
  int distance{1};
  MovementSpeed speed = MovementSpeed::Normal;
};