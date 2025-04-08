#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeSaveAccessCommand final : IEventCommand {
  ChangeSaveAccessCommand() = default;
  explicit ChangeSaveAccessCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeSaveAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Save_Access; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeSaveAccessCommand>(*this); }

  AccessMode access = AccessMode::Disable;
};
