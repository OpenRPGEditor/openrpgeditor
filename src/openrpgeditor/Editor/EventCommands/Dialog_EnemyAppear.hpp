#pragma once
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/EnemyAppear.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Dialog_EnemyAppear : IEventDialogController {
  Dialog_EnemyAppear() = delete;
  explicit Dialog_EnemyAppear(const std::string& name, const std::shared_ptr<EnemyAppearCommand>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new EnemyAppearCommand());
    }
    m_troopMemberSelection = m_command->troopMember;
    m_command->m_troopId = m_troopId;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_troopId = 0;
  int m_troopMemberSelection;

  bool m_confirmed{false};
  std::shared_ptr<EnemyAppearCommand> m_command;
};