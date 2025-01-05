#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct EndCommand final : IEventCommand {
  EndCommand() = default;
  explicit EndCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~EndCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End; }

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
