#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EndCommand final : IEventCommand {
  EndCommand() = default;
  explicit EndCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<EndCommand>(*this); }

  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::Conditional_Branch || code == EventCode::Else) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] bool reverseSelection() const override { return true; }
  bool m_collapsed{false};
};
