#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ControlTimerCommand final : IEventCommand {
  ControlTimerCommand() = default;
  explicit ControlTimerCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ControlTimerCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Timer; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ControlTimerCommand>(*this); }

  TimerControl control = TimerControl::Start;
  int seconds = 0;
};
