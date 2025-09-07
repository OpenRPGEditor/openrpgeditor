#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeEXP.hpp"

struct Dialog_ChangeEXP : IEventDialogController {
  Dialog_ChangeEXP() = delete;
  explicit Dialog_ChangeEXP(const std::string& name, const std::shared_ptr<ChangeEXPCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeEXPCommand());
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
  std::shared_ptr<ChangeEXPCommand> m_command;
};