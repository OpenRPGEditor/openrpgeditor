#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct FlashScreenCommand : IEventCommand {
  ~FlashScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Flash_Screen; }
  struct {
    int r;
    int g;
    int b;
    int intensity;
  } colors;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
