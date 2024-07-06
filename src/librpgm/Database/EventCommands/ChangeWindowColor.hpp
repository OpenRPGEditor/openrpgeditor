#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeWindowColorCommand final : IEventCommand {
  ChangeWindowColorCommand() = default;
  explicit ChangeWindowColorCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeWindowColorCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Window_Color; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int r = 255;
  int g = 255;
  int b = 255;
};
