#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowBalloonIconCommand : IEventCommand {
  ~ShowBalloonIconCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Balloon_Icon; }
  int id;
  BalloonIcon index;
  bool waitForCompletion;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
