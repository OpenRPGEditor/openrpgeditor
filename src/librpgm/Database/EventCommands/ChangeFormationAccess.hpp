#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeFormationAccessCommand final : IEventCommand {
  ChangeFormationAccessCommand() = default;
  explicit ChangeFormationAccessCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeFormationAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Formation_Access; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  AccessMode access = AccessMode::Disable;
};
