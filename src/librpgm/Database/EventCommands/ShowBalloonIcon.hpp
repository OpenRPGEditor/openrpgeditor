#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct ShowBalloonIconCommand final : IEventCommand {
  ShowBalloonIconCommand() = default;
  explicit ShowBalloonIconCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~ShowBalloonIconCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Show_Balloon_Icon; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int id = -1;
  BalloonIcon index = BalloonIcon::Exclamation;
  bool waitForCompletion = false;
};
