#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ChangeMenuAccessCommand final : IEventCommand {
  ChangeMenuAccessCommand() = default;
  explicit ChangeMenuAccessCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ChangeMenuAccessCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Menu_Access; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ChangeMenuAccessCommand>(*this); }

  AccessMode access;
};