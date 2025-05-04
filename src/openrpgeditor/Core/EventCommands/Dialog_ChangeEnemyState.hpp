#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeEnemyState.hpp"
#include "Database/States.hpp"

struct Dialog_ChangeEnemyState : IEventDialogController {
  Dialog_ChangeEnemyState() = delete;
  explicit Dialog_ChangeEnemyState(const std::string& name, const std::shared_ptr<ChangeEnemyStateCommand>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeEnemyStateCommand());
    }
    m_troopMemberSelection = m_command->troopMember;
    m_troopMemberOp = static_cast<int>(m_command->troopMemberOp);
    m_state = m_command->state;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_troopId;
  int m_troopMemberSelection;
  int m_troopMemberOp; // Party Member Operation
  int m_state;

  bool m_confirmed{false};
  std::optional<ObjectPicker<State>> m_statePicker;
  std::shared_ptr<ChangeEnemyStateCommand> m_command;
};