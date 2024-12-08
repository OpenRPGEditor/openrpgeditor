#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/EnemyRecoverAll.hpp"
#include "Database/EventCommands/RecoverAll.hpp"

struct Dialog_EnemyRecoverAll : IEventDialogController {
  Dialog_EnemyRecoverAll() = delete;
  explicit Dialog_EnemyRecoverAll(const std::string& name, const std::shared_ptr<EnemyRecoverAllCommand>& cmd = nullptr) : IEventDialogController(name), command(cmd) {
    if (cmd == nullptr) {
      command.reset(new EnemyRecoverAllCommand());
    }
    m_troop_selection = command->troop;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_troop_selection = 0;

  bool m_confirmed{false};
  std::shared_ptr<EnemyRecoverAllCommand> command;
  std::tuple<bool, bool> result;
};