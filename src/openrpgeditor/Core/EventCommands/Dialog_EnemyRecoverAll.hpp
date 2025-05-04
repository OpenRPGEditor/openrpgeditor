#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/EnemyRecoverAll.hpp"

struct Dialog_EnemyRecoverAll : IEventDialogController {
  Dialog_EnemyRecoverAll() = delete;
  explicit Dialog_EnemyRecoverAll(const std::string& name, const std::shared_ptr<EnemyRecoverAllCommand>& cmd = nullptr, int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new EnemyRecoverAllCommand());
    }
    m_troopMemberSelection = m_command->troopMember;
    m_command->m_troopId = m_troopId;
  }

  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

  void setTroopId(const int troopId) { m_troopId = troopId; }

private:
  int m_troopId = 0;
  int m_troopMemberSelection = 0;

  bool m_confirmed{false};
  std::shared_ptr<EnemyRecoverAllCommand> m_command;
  std::tuple<bool, bool> result;
};