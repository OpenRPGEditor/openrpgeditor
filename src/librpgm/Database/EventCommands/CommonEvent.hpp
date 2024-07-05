#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct CommonEventCommand final : IEventCommand {
  CommonEventCommand() = default;
  explicit CommonEventCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(event);
  }

  ~CommonEventCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Common_Event; }
  int event = 1;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
