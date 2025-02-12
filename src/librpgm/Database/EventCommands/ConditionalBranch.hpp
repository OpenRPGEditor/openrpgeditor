#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ConditionalBranchCommand final : IEventCommand {
  ConditionalBranchCommand() = default;
  explicit ConditionalBranchCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ConditionalBranchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Conditional_Branch; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ConditionalBranchCommand>(*this); }
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  [[nodiscard]] std::string conditionalFormat(const std::string& text) const;
  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::Else || code == EventCode::End) && *codeIndent == indent();
  }

  [[nodiscard]] constexpr int partnerCount() const override { return 2; }

  [[nodiscard]] constexpr bool hasPartner() const override { return true; }

  bool hasVariable(int targetId) override { return type == ConditionType::Variable && (variable.id == targetId || variable.otherId == targetId); };

  ConditionType type{};
  struct {
    int switchIdx{1};
    ValueControl checkIfOn = ValueControl::ON;
  } globalSwitch{};
  struct {
    int id{};
    VariableComparisonSource source{};
    VariableComparisonType comparison{};
    union {
      int constant;
      int otherId{};
    };
  } variable{};
  struct {
    ValueControl checkIfOn;
  } selfSwitch{};
  struct {
    TimerComparisonType comparison{};
    int sec{};
  } timer{};
  struct {
    int id{};
    ActorConditionType type{};
    int checkId{};
  } actor{};
  struct {
    int id{};
    EnemyConditionType type{};
    int stateId{}; // For "State" condition
  } enemy;
  struct {
    int id{};
    Direction facing{};
  } character{};
  struct {
    VehicleType id{};
  } vehicle;
  struct {
    GoldComparisonType type{};
    int value{};
  } gold{};
  struct {
    int id{};
  } item{};
  /* For Weapon and Armor conditions */
  struct {
    int equipId{};
    bool includeEquipment{};
  } equip{};
  Button button;
  std::string selfSw; // A, B, C, D
  std::string name;
  std::string script;

  bool m_collapsed{false};
};

struct ElseCommand final : IEventCommand {
  ElseCommand() = default;
  explicit ElseCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ElseCommand>(*this); }

  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  [[nodiscard]] bool isCollapseEnd() const override { return false; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::Conditional_Branch || code == EventCode::End) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr int partnerCount() const override { return 2; }
  [[nodiscard]] bool reverseSelection() const override { return m_reverseSelection; }
  void setReverseSelection(const bool reverseSelection) override { m_reverseSelection = reverseSelection; }
  bool m_reverseSelection = false;
  bool m_collapsed{false};
};