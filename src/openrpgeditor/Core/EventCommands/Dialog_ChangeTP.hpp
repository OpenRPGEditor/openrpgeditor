#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeTP.hpp"

struct Dialog_ChangeTP : IEventDialogController {
  Dialog_ChangeTP() = delete;
  explicit Dialog_ChangeTP(const std::string& name, const std::shared_ptr<ChangeTPCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeTPCommand());
    }

    m_comparison = static_cast<int>(m_command->comparison);
    m_quantityOp = static_cast<int>(m_command->quantityOp);
    m_quantitySource = static_cast<int>(m_command->quantitySource);
    if (m_command->comparison == ActorComparisonSource::Fixed) {
      m_value = m_command->value;
    } else {
      m_valueVar = m_command->value;
    }
    if (m_command->quantitySource == QuantityChangeSource::Constant) {
      m_quantity = m_command->quantity;
    } else {
      m_quantityVar = m_command->quantity;
    }
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();

  int m_comparison;
  int m_value{1};
  int m_valueVar{1};
  int m_quantityOp;
  int m_quantitySource;
  int m_quantity{1};
  int m_quantityVar{1};
  bool m_isOperand{false};

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeTPCommand> m_command;
};