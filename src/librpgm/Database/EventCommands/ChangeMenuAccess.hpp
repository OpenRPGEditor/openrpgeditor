#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeMenuAccessCommand final : IEventCommand {
  ChangeMenuAccessCommand() = default;
  explicit ChangeMenuAccessCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  AccessMode access;
};