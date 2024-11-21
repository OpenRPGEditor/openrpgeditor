#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintPictureCommand final : IEventCommand {
  TintPictureCommand() = default;
  explicit TintPictureCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~TintPictureCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Picture; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  int picture{1};

  struct {
    int r{0};
    int g{0};
    int b{0};
    int gray{0};
  } color;

  int duration{60};
  bool waitForCompletion{true};

};