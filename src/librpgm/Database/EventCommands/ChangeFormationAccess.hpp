#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeFormationAccessCommand final : IEventCommand {
  ChangeFormationAccessCommand() = default;
  explicit ChangeFormationAccessCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(access);
  }
  ~ChangeFormationAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Formation_Access; }
  AccessMode access = AccessMode::Disable;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
