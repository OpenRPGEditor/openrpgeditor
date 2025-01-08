#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FlashScreenCommand final : IEventCommand {
  FlashScreenCommand() = default;
  explicit FlashScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~FlashScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Flash_Screen; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<FlashScreenCommand>(*this); }

  struct {
    int r{255};
    int g{255};
    int b{255};
    int intensity{170};
  } color;
  int duration{60};
  bool waitForCompletion{true};
};
