#pragma once
#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeHP.hpp"
#include "Database/EventCommands/ChangeLevel.hpp"
#include "Database/Skills.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_ChangeLevel : IEventDialogController {
  Dialog_ChangeLevel() = delete;
  explicit Dialog_ChangeLevel(const std::string& name, const std::shared_ptr<ChangeLevelCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeLevelCommand());
    }

    m_comparison = static_cast<int>(m_command->comparison);
    m_quantityOp = static_cast<int>(m_command->quantityOp);
    m_quantitySource = static_cast<int>(m_command->quantitySource);
    m_showLevelUp = m_command->showLevelUp;

    if (m_command->comparison == ActorComparisonSource::Variable)
      m_valueVar = m_command->value;
    else
      m_value = m_command->value;

    if (m_command->quantitySource == QuantityChangeSource::Variable)
      m_quantityVar = m_command->quantity;
    else
      m_quantity = m_command->quantity;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_comparison;
  int m_value;
  int m_valueVar{1};
  int m_quantityOp;
  int m_quantitySource;
  int m_quantity;
  int m_quantityVar{1};
  bool m_showLevelUp{false};
  bool m_isOperand{false};

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeLevelCommand> m_command;
};