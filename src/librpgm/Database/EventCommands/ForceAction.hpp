#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ForceActionCommand final : IEventCommand {
  ForceActionCommand() = default;
  explicit ForceActionCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ForceActionCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Force_Action; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ForceActionCommand>(*this); }

  SubjectComparisonSource sourceComparison;
  int source;
  int skill;
  int target;
};
