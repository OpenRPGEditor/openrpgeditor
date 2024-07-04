#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintScreenCommand : IEventCommand {
  ~TintScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Screen; }
  struct {
    int r;
    int g;
    int b;
    int gray;
  } colors;
  int duration;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
