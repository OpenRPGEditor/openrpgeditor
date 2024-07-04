#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct BattleProcessingCommand : IEventCommand {
  ~BattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Battle_Processing; }
  BattleProcessType type;
  int id; // troop id if type == 0; variable id if type == 2
  bool canEscape;
  bool canLose;

  [[nodiscard]] std::string stringRep(const Database& db) const override;
};

struct IfWinCommand : IEventCommand {
  ~IfWinCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Win; }
};

struct IfEscapeCommand : IEventCommand {
  ~IfEscapeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Escape; }
};

struct IfLoseCommand : IEventCommand {
  ~IfLoseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Lose; }
};

struct EndBattleProcessingCommand : IEventCommand {
  ~EndBattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_Battle_Processing; }
};