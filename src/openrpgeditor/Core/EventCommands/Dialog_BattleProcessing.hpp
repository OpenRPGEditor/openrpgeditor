#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/BattleProcessing.hpp"
#include "Database/Troops.hpp"

struct Dialog_BattleProcessing : IEventDialogController {
  Dialog_BattleProcessing() = delete;
  explicit Dialog_BattleProcessing(const std::string& name, const std::shared_ptr<BattleProcessingCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new BattleProcessingCommand());
    }

    m_type = static_cast<int>(command->type);
    m_id = command->id;
    m_canEscape = command->canEscape;
    m_canLose = command->canLose;
  }
  std::tuple<bool, bool> draw() override;
  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_type;
  int m_id;
  bool m_canEscape;
  bool m_canLose;

  int m_var_selection{1};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Troop>> enemy_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<BattleProcessingCommand> command;
  std::tuple<bool, bool> result;
};