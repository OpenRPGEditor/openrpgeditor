#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintPictureCommand final : IEventCommand {
  TintPictureCommand() = default;
  explicit TintPictureCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~TintPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Picture; }
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int picture;

  struct {
    int r;
    int g;
    int b;
    int gray;
  } color;

  int duration;
  bool waitForCompletion;

};