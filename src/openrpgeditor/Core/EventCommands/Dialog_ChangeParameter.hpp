#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeParameter.hpp"

struct Dialog_ChangeParameter : IEventDialogController {
  Dialog_ChangeParameter() = delete;
  explicit Dialog_ChangeParameter(const std::string& name, const std::shared_ptr<ChangeParameterCommand>& cmd = nullptr) : IEventDialogController(name), m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeParameterCommand());
    }

    m_comparison = static_cast<int>(m_command->comparison);
    m_quantityOp = static_cast<int>(m_command->quantityOp);
    m_quantitySource = static_cast<int>(m_command->quantitySource);
    m_parameterSource = static_cast<int>(m_command->param);

    if (m_command->comparison == ActorComparisonSource::Variable)
      m_value_var = m_command->value;
    else
      m_value = m_command->value;

    if (m_command->quantitySource == QuantityChangeSource::Variable)
      m_quantity_var = m_command->quantity;
    else
      m_quantity = m_command->quantity;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_comparison;
  int m_value{1};
  int m_value_var{1};
  int m_quantityOp;
  int m_quantitySource;
  int m_quantity{1};
  int m_quantity_var{1};
  bool m_isOperand{false};
  int m_parameterSource;

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<VariableSwitchPicker> m_picker;
  std::shared_ptr<ChangeParameterCommand> m_command;
  std::tuple<bool, bool> m_result;
};