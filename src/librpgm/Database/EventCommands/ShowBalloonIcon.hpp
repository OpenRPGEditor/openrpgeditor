#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowBalloonIconCommand final : IEventCommand {
  ShowBalloonIconCommand() = default;
  explicit ShowBalloonIconCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {
    parameters[0].get_to(id);
    parameters[1].get_to(index);
    parameters[2].get_to(waitForCompletion);
  }
  ~ShowBalloonIconCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Balloon_Icon; }
  int id = -1;
  BalloonIcon index = BalloonIcon::Exclamation;
  bool waitForCompletion = false;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
};
