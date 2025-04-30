#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeEncounterDisableCommand final : IEventCommand {
  ChangeEncounterDisableCommand() = default;
  explicit ChangeEncounterDisableCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeEncounterDisableCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Encounter; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeEncounterDisableCommand>(*this); }

  AccessMode access = AccessMode::Disable;
};
