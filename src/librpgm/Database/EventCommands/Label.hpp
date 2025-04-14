#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct LabelCommand final : IEventCommand {
  LabelCommand() = default;
  explicit LabelCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~LabelCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Label; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<LabelCommand>(*this); }
  std::string label;

  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::Label || code == EventCode::Event_Dummy) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  [[nodiscard]] bool isCollapseEnd() const override { return false; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  bool isTerminatingPartner(const EventCode code, const std::optional<int>& codeIndent) const override {
    if (!codeIndent) {
      return false;
    }
    return (code == EventCode::Label || code == EventCode::Event_Dummy) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr int partnerCount() const override { return 2; }
  [[nodiscard]] bool reverseSelection() const override { return m_reverseSelection; }
  void setReverseSelection(const bool reverseSelection) override { m_reverseSelection = reverseSelection; }
  bool m_reverseSelection = false;
  bool m_collapsed{false};
};