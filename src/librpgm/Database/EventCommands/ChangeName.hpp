#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeNameCommand final : IEventCommand {
  ~ChangeNameCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Name; }

  int actor;
  std::string name;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
