#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintPictureCommand : IEventCommand {
  ~TintPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Picture; }
  int picture;

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