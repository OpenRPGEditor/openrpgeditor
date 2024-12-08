#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"

#include "Database/Actor.hpp"
#include "Database/EventCommands/ChangeState.hpp"
#include "Database/State.hpp"

struct Dialog_ChangeState : IEventDialogController {
  Dialog_ChangeState() = delete;
  explicit Dialog_ChangeState(const std::string& name, const std::shared_ptr<ChangeStateCommand>& cmd = nullptr) : IEventDialogController(name), m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeStateCommand());
    }

    m_state = static_cast<int>(m_command->state);
    m_stateOp = static_cast<int>(m_command->stateOp);
    m_comparison = static_cast<int>(m_command->comparison);

    if (m_command->comparison == ActorComparisonSource::Variable)
      m_actorVar = m_command->value;
    else
      m_actor = m_command->value;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_state;
  int m_stateOp;
  int m_comparison;
  int m_actor{1};
  int m_actorVar{1};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> m_actorPicker;
  std::optional<ObjectPicker<State>> m_statePicker;
  std::optional<VariableSwitchPicker> m_picker;
  std::shared_ptr<ChangeStateCommand> m_command;
  std::tuple<bool, bool> m_result;
};