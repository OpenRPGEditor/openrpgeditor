#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeMenuAccessCommand final : IEventCommand {
  ChangeMenuAccessCommand() = default;
  explicit ChangeMenuAccessCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(access);
  }
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  AccessMode access;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};