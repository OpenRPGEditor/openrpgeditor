#pragma once
#include "Database/EventCommands/IEventCommand.hpp"

struct ConditionalBranchCommand : IEventCommand {
  ~ConditionalBranchCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Conditional_Branch; }
  ConditionType type{};
  struct {
    int switchIdx{};
    ValueControl checkIfOn;
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

  [[nodiscard]] std::string stringRep(const Database& db) const override;
  [[nodiscard]] std::string conditionalFormat(const std::string& text) const;
};

struct ElseCommand : IEventCommand {
  ~ElseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Else; }
};