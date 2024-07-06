#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct CommonEventCommand final : IEventCommand {
  CommonEventCommand() = default;
  explicit CommonEventCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~CommonEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Common_Event; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int event = 1;
};
