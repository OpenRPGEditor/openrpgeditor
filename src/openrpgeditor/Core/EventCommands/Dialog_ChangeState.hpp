#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/EventCommands/ChangeState.hpp"
#include "Database/States.hpp"
#include "Database/Actors.hpp"

struct Dialog_ChangeState : IEventDialogController {
  Dialog_ChangeState() = delete;
  explicit Dialog_ChangeState(const std::string& name,
                              const std::shared_ptr<ChangeStateCommand>& cmd = nullptr)
  : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeStateCommand());
    }

    m_state = static_cast<int>(command->state);
    m_stateOp = static_cast<int>(command->stateOp);
    m_comparison = static_cast<int>(command->comparison);

    if (command->comparison == ActorComparisonSource::Variable)
      m_actor_var = command->value;
    else
      m_actor = command->value;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_state;
  int m_stateOp;
  int m_comparison;
  int m_actor{1};
  int m_actor_var{1};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::optional<ObjectPicker<State>> state_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ChangeStateCommand> command;
  std::tuple<bool, bool> result;
};