#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ConditionalBranchCommand final : IEventCommand {
  ConditionalBranchCommand() = default;
  explicit ConditionalBranchCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~ConditionalBranchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Conditional_Branch; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;
  [[nodiscard]] std::string conditionalFormat(const std::string& text) const;

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
};

struct ElseCommand final : IEventCommand {
  ElseCommand() = default;
  explicit ElseCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
};