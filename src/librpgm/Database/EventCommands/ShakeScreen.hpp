#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShakeScreenCommand final : IEventCommand {
  ShakeScreenCommand() = default;
  explicit ShakeScreenCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ShakeScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shake_Screen; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int power;
  int speed;
  int duration;
  bool waitForCompletion;
};
