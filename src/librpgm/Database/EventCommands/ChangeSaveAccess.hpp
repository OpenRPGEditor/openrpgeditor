#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeSaveAccessCommand final : IEventCommand {
  ChangeSaveAccessCommand() = default;
  explicit ChangeSaveAccessCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(access);
  }
  ~ChangeSaveAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Save_Access; }
  AccessMode access = AccessMode::Disable;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
