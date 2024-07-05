#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct InputNumberCommand final : IEventCommand {
  InputNumberCommand() = default;
  explicit InputNumberCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(variable);
    parameters[1].get_to(digits);
  }

  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
  int variable;
  int digits;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
