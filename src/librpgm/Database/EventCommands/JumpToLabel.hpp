#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct JumpToLabelCommand final : IEventCommand {
  JumpToLabelCommand() = default;
  explicit JumpToLabelCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~JumpToLabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Jump_To_Label; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<JumpToLabelCommand>(*this); }

  std::string label;
};