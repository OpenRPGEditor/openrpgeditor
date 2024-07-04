#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ChangeProfileCommand : IEventCommand {
  ~ChangeProfileCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Change_Profile; }

  int actor;
  std::string profile;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
