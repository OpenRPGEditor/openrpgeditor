#pragma once
#include "Database/EventCommands/BattleProcessing.hpp"
#include "Database/Troops.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_BattleProcessing : IEventDialogController {
  Dialog_BattleProcessing() = delete;
  explicit Dialog_BattleProcessing(const std::string& name, const std::shared_ptr<BattleProcessingCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new BattleProcessingCommand());
    }

    m_type = static_cast<int>(m_command->type);
    m_id = m_command->id;
    m_canEscape = m_command->canEscape;
    m_canLose = m_command->canLose;
  }
  std::tuple<bool, bool> draw() override;
  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_type;
  int m_id;
  bool m_canEscape;
  bool m_canLose;

  int m_varSelection{1};

  bool m_confirmed{false};
  std::optional<TroopPicker> m_enemyPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<BattleProcessingCommand> m_command;
};