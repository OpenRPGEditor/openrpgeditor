#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintScreenCommand final : IEventCommand {
  TintScreenCommand() = default;
  explicit TintScreenCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~TintScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Screen; }
  struct {
    int r;
    int g;
    int b;
    int gray;
  } color;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
