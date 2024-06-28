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

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + "Battle Processing : "
    + (type == BattleProcessType::Direct_designation ? "{}" : type == BattleProcessType::Designation_with_variables ? "{[]}" : "Same as Random Encounter")
    + ColorFormatter::popColor();
  }
};

struct IfWinCommand : IEventCommand {
  ~IfWinCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Win; }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " If Win" + ColorFormatter::popColor();
  }
};

struct IfEscapeCommand : IEventCommand {
  ~IfEscapeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Escape; }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " If Escape" + ColorFormatter::popColor();
  }
};

struct IfLoseCommand : IEventCommand {
  ~IfLoseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Lose; }

  [[nodiscard]] std::string stringRep(const Database& db) const override {
    return indentText(indent) + symbol(code()) + ColorFormatter::getColorCode(code())
    + " If Lose" + ColorFormatter::popColor();
  }
};