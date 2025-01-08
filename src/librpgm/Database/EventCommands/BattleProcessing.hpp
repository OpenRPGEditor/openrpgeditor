#pragma once
#include "Database/EventCommands/IEventCommand.hpp"
#include <format>

struct BattleProcessingCommand final : IEventCommand {
  BattleProcessingCommand() = default;
  explicit BattleProcessingCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters);
  ~BattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::Battle_Processing; }
  void serializeParameters(nlohmann::ordered_json& out) const override;
  [[nodiscard]] std::string stringRep(const Database& db) const override;

  BattleProcessType type = BattleProcessType::Direct_designation;
  int id{1}; // troop id if type == 0; variable id if type == 2
  bool canEscape{false};
  bool canLose{false};

  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<BattleProcessingCommand>(*this); }
};

struct IfWinCommand final : IEventCommand {
  IfWinCommand() = default;
  explicit IfWinCommand(const std::optional<int>& indent, const nlohmann::json& parameters) : IEventCommand(indent, parameters) {}
  ~IfWinCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Win; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<IfWinCommand>(*this); }
};

struct IfEscapeCommand final : IEventCommand {
  IfEscapeCommand() = default;
  explicit IfEscapeCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~IfEscapeCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Escape; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<IfEscapeCommand>(*this); }
};

struct IfLoseCommand final : IEventCommand {
  IfLoseCommand() = default;
  explicit IfLoseCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~IfLoseCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::If_Lose; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<IfLoseCommand>(*this); }
};

struct EndBattleProcessingCommand final : IEventCommand {
  EndBattleProcessingCommand() = default;
  explicit EndBattleProcessingCommand(const std::optional<int>& indent, const nlohmann::ordered_json& parameters) : IEventCommand(indent, parameters) {}
  ~EndBattleProcessingCommand() override = default;
  [[nodiscard]] EventCode code() const override { return EventCode::End_del_Battle_Processing; }
  std::shared_ptr<IEventCommand> clone() const override { return std::make_shared<EndBattleProcessingCommand>(*this); }
};