#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlSwitches : IEventCommand {
  ControlSwitches() = default;
  explicit ControlSwitches(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ControlSwitches>(*this); }

  int start{1};
  int end{1};
  ValueControl turnOff = ValueControl::ON;
};
