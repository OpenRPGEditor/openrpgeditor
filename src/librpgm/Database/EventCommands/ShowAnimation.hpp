#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowAnimationCommand : IEventCommand {
  ~ShowAnimationCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Animation; }
  int character;
  int animation;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
