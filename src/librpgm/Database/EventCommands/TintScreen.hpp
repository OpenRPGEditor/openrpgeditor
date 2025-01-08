#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintScreenCommand final : IEventCommand {
  TintScreenCommand() = default;
  explicit TintScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~TintScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Screen; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<TintScreenCommand>(*this); }

  struct {
    int r;
    int g;
    int b;
    int gray;
  } color;
  int duration;
  bool waitForCompletion;
};
