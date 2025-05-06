#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct RepeatAboveCommand final : IEventCommand {
  RepeatAboveCommand() = default;
  explicit RepeatAboveCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~RepeatAboveCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Repeat_Above; }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr int partnerCount() const override { return 1; }
  [[nodiscard]] bool reverseSelection() const override { return true; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<RepeatAboveCommand>(*this); }

  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }
    return code == EventCode::Loop && indent() == *codeIndent;
  }
};
