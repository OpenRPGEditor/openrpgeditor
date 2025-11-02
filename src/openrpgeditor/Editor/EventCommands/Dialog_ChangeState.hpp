#pragma once
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeState.hpp"
#include "Database/State.hpp"

struct Dialog_ChangeState : IEventDialogController {
  Dialog_ChangeState() = delete;
  explicit Dialog_ChangeState(const std::string& name, const std::shared_ptr<ChangeStateCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeStateCommand());
    }

    m_state = m_command->state;
    m_stateOp = static_cast<int>(m_command->stateOp);
    m_comparison = static_cast<int>(m_command->comparison);

    if (m_command->comparison == ActorComparisonSource::Variable)
      m_var = m_command->value;
    else
      m_actor = m_command->value;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_state;
  int m_stateOp;
  int m_comparison;
  int m_actor{1};
  int m_var{1};

  bool m_confirmed{false};
  std::optional<ActorPicker> m_actorPicker;
  std::optional<StatePicker> m_statePicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeStateCommand> m_command;
};