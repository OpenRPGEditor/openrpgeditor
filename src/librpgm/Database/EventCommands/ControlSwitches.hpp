#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ControlSwitches : IEventCommand {
  ControlSwitches() = default;
  explicit ControlSwitches(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(start);
    parameters[1].get_to(end);
    parameters[2].get_to(turnOff); // It's inverted because why the fuck not
  }
  ~ControlSwitches() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Control_Switches; }
  int start = 1;
  int end = 1;
  ValueControl turnOff = ValueControl::ON;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
