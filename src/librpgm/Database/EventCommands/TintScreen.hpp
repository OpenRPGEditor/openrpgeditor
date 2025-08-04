#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct TintScreenCommand final : IEventCommand {
  TintScreenCommand() = default;
  explicit TintScreenCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~TintScreenCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Tint_Screen; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<TintScreenCommand>(*this); }

  struct {
    int r = 0;
    int g = 0;
    int b = 0;
    int gray = 0;
  } color;
  int duration = 60;
  bool waitForCompletion;
};
