#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct BattleProcessingCommand final : IEventCommand {
  BattleProcessingCommand() = default;
  explicit BattleProcessingCommand(const std::optional<int>& indent, nlohmann::json& parameters);
  ~BattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Battle_Processing; }
  BattleProcessType type;
  int id; // troop id if type == 0; variable id if type == 2
  bool canEscape;
  bool canLose;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct IfWinCommand final : IEventCommand {
  IfWinCommand() = default;
  explicit IfWinCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~IfWinCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Win; }
};

struct IfEscapeCommand final : IEventCommand {
  IfEscapeCommand() = default;
  explicit IfEscapeCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~IfEscapeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Escape; }
};

struct IfLoseCommand final : IEventCommand {
  IfLoseCommand() = default;
  explicit IfLoseCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~IfLoseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Lose; }
};

struct EndBattleProcessingCommand final : IEventCommand {
  EndBattleProcessingCommand() = default;
  explicit EndBattleProcessingCommand(const std::optional<int>& indent, nlohmann::json& parameters)
  : IEventCommand(indent, parameters) {}
  ~EndBattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_Battle_Processing; }
};