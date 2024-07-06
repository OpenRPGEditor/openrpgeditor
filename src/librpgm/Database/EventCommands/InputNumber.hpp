#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct InputNumberCommand final : IEventCommand {
  InputNumberCommand() = default;
  explicit InputNumberCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~InputNumberCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Input_Number; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int variable = 1;
  int digits = 1;
};
