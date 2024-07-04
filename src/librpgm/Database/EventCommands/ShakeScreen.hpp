#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShakeScreenCommand : IEventCommand {
  ~ShakeScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Shake_Screen; }
  int power;
  int speed;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
