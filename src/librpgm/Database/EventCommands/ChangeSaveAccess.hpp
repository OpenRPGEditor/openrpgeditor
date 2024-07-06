#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeSaveAccessCommand final : IEventCommand {
  ChangeSaveAccessCommand() = default;
  explicit ChangeSaveAccessCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ChangeSaveAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Save_Access; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  AccessMode access = AccessMode::Disable;
};
