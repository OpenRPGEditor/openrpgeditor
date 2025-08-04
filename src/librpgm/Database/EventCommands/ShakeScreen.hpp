#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShakeScreenCommand final : IEventCommand {
  ShakeScreenCommand() = default;
  explicit ShakeScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ShakeScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shake_Screen; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ShakeScreenCommand>(*this); }

  int power = 5;
  int speed = 5;
  int duration = 60;
  bool waitForCompletion{true};
};
