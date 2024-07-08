#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct BattleProcessingCommand final : IEventCommand {
  BattleProcessingCommand() = default;
  explicit BattleProcessingCommand(const std::optional<int>& indent, const nlohmann::json& parameters);
  ~BattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Battle_Processing; }
  void serializeParameters(nlohmann::json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  BattleProcessType type = BattleProcessType::Direct_designation;
  int id{1}; // troop id if type == 0; variable id if type == 2
  bool canEscape{false};
  bool canLose{false};
};

struct IfWinCommand final : IEventCommand {
  IfWinCommand() = default;
  explicit IfWinCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~IfWinCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Win; }
};

struct IfEscapeCommand final : IEventCommand {
  IfEscapeCommand() = default;
  explicit IfEscapeCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~IfEscapeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Escape; }
};

struct IfLoseCommand final : IEventCommand {
  IfLoseCommand() = default;
  explicit IfLoseCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~IfLoseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Lose; }
};

struct EndBattleProcessingCommand final : IEventCommand {
  EndBattleProcessingCommand() = default;
  explicit EndBattleProcessingCommand(const std::optional<int>& indent, const nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~EndBattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_Battle_Processing; }
};