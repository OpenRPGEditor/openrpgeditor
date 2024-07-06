#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FlashScreenCommand final : IEventCommand {
  FlashScreenCommand() = default;
  explicit FlashScreenCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~FlashScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Flash_Screen; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  struct {
    int r;
    int g;
    int b;
    int intensity;
  } color;
  int duration;
  bool waitForCompletion;
};
