#pragma once
#include "Database/Database.hpp"
#include "Database/EventCommands/IEventCommand.hpp"

struct ConditionalBranchCommand final : IEventCommand {
  ConditionalBranchCommand() = default;
  explicit ConditionalBranchCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ConditionalBranchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Conditional_Branch; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<ConditionalBranchCommand>(*this); }
  [[nodiscard]] std::string stringRep(const Database& db, bool colored = true) const override;
  [[nodiscard]] std::string conditionalFormat(const std::string& text, bool colored) const;
  [[nodiscard]] constexpr bool collapsable() const override { return true; }
  [[nodiscard]] bool isCollapsed() const override { return m_collapsed; }
  void setCollapsed(const bool collapsed) override { m_collapsed = collapsed; }
  bool isPartner(const EventCode code, const std::optional<int>& codeIndent) override {
    if (!codeIndent) {
      return false;
    }

    return (code == EventCode::Else || code == EventCode::End) && *codeIndent == indent();
  }

  bool isTerminatingPartner(const EventCode code, const std::optional<int>& codeIndent) const override {
    if (!codeIndent) {
      return false;
    }
    return (code == EventCode::End) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr int partnerCount() const override { return 2; }

  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr bool isParent() const override { return true; }

  bool hasReference(int targetId, SearchType searchType) override {
    if (searchType == SearchType::Variable) {
      if (type == ConditionType::Variable) {
        if (variable.id == targetId) {
          return true;
        }
        return variable.source == VariableComparisonSource::Variable && variable.otherId == targetId;
      }
      if (type == ConditionType::Script) {
        std::string cnst = Database::instance()->gameConstants->variables[targetId];
        if (cnst.empty()) {
          return false;
        }
        if (script.contains("gameVariables") && script.contains(cnst)) {
          return true;
        }
      }
    }
    if (searchType == SearchType::Switch) {
      if (type == ConditionType::Script) {
        std::string cnst = Database::instance()->gameConstants->switches[targetId];
        if (cnst.empty()) {
          // Return a warning, we want everything to have a constant
          return false;
        }
        if (script.contains("gameSwitches") && script.contains(cnst)) {
          return true;
        }
      }
      return type == ConditionType::Switch && globalSwitch.switchIdx == targetId;
    }

    if (searchType == SearchType::ConditionSwitchOFF) {
      if (type == ConditionType::Switch) {
        if (globalSwitch.checkIfOn == ValueControl::OFF) {
          return true;
        }
      }
    }

    if (searchType == SearchType::Armors || searchType == SearchType::Weapons) {
      if (type == ConditionType::Armor || type == ConditionType::Weapon) {
        return equip.equipId == targetId;
      }
    }
    if (searchType == SearchType::Items) {
      if (type == ConditionType::Item) {
        return item.id == targetId;
      }
    }
    if (searchType == SearchType::Enemy) {
      if (type == ConditionType::Enemy) {
        return enemy.id == targetId;
      }
    }
    if (searchType == SearchType::State) {
      if (type == ConditionType::Actor) {
        if (actor.type == ActorConditionType::State) {
          return actor.checkId == targetId;
        }
      }
      if (type == ConditionType::Enemy) {
        return enemy.stateId == targetId;
      }
    }
    if (searchType == SearchType::Actor) {
      if (type == ConditionType::Actor) {
        return actor.id == targetId;
      }
    }
    if (searchType == SearchType::Skill) {
      if (type == ConditionType::Actor) {
        if (actor.type == ActorConditionType::Skill) {
          return actor.checkId == targetId;
        }
      }
    }
    if (searchType == SearchType::Class) {
      if (type == ConditionType::Actor) {
        if (actor.type == ActorConditionType::Class) {
          return actor.checkId == targetId;
        }
      }
    }

    return false;
  };

  bool setReference(int targetId, int newId, SearchType searchType) override {
    if (hasReference(targetId, searchType)) {

      if (searchType == SearchType::Variable) {
        if (type == ConditionType::Variable) {
          if (variable.id == targetId) {
            variable.id = newId;
          }
          if (variable.source == VariableComparisonSource::Variable && variable.otherId == targetId) {
            variable.id = newId;
          }
          return true;
        }
      }
      if (searchType == SearchType::Switch) {
        if (type == ConditionType::Switch && globalSwitch.switchIdx == targetId) {
          globalSwitch.switchIdx = newId;
        }
        return true;
      }
      if (searchType == SearchType::Armors || searchType == SearchType::Weapons) {
        if (equip.equipId == targetId) {
          equip.equipId = newId;
        }
      }
      if (searchType == SearchType::Items) {
        if (type == ConditionType::Item && item.id == targetId) {
          item.id = newId;
        }
      }
      if (searchType == SearchType::Enemy) {
        if (type == ConditionType::Enemy && enemy.id == targetId) {
          enemy.id = newId;
        }
      }
      if (searchType == SearchType::State) {
        if (type == ConditionType::Actor) {
          if (actor.type == ActorConditionType::State && actor.checkId == targetId) {
            actor.checkId = newId;
          }
        }
        if (type == ConditionType::Enemy && enemy.stateId == targetId) {
          enemy.id = newId;
        }
      }

      if (searchType == SearchType::Actor) {
        if (actor.id == targetId) {
          actor.id = newId;
        }
      }
      if (searchType == SearchType::Skill) {
        if (type == ConditionType::Actor) {
          if (actor.type == ActorConditionType::Skill && actor.checkId == targetId) {
            actor.checkId = newId;
          }
        }
      }
      if (searchType == SearchType::Class) {
        if (type == ConditionType::Actor) {
          if (actor.type == ActorConditionType::Class && actor.checkId == targetId) {
            actor.checkId = newId;
          }
        }
      }
      return true;
    }
    return false;
  }

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
  int m_troopId{0};
};

struct ElseCommand final : IEventCommand {
  ElseCommand() = default;
  explicit ElseCommand(const std::optional<int>& indent)
  : IEventCommand(indent) {}
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
  bool isTerminatingPartner(const EventCode code, const std::optional<int>& codeIndent) const override {
    if (!codeIndent) {
      return false;
    }
    return (code == EventCode::End) && *codeIndent == indent();
  }
  [[nodiscard]] constexpr bool hasPartner() const override { return true; }
  [[nodiscard]] constexpr int partnerCount() const override { return 2; }
  [[nodiscard]] bool reverseSelection() const override { return m_reverseSelection; }
  void setReverseSelection(const bool reverseSelection) override { m_reverseSelection = reverseSelection; }
  bool m_reverseSelection = false;
  bool m_collapsed{false};
};