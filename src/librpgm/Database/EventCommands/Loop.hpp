#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LoopCommand final : IEventCommand {
  LoopCommand() = default;
  explicit LoopCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters)
  : IEventCommand(indent) {}
  ~LoopCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Loop; }
  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr int partnerCount() const override { return 1; }
  [[nodiscard]] constexpr bool isParent() const override { return true; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<LoopCommand>(*this); }

  [[nodiscard]] bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }
    return code == EventCode::Repeat_Above && indent() == *codeIndent;
  }

  bool m_collapsed = false;
};
