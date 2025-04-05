#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Class.hpp"
#include "Database/EventCommands/ChangeGold.hpp"
#include "Database/EventCommands/EnemyAppear.hpp"

struct Dialog_EnemyAppear : IEventDialogController {
  Dialog_EnemyAppear() = delete;
  explicit Dialog_EnemyAppear(const std::string& name, const std::shared_ptr<EnemyAppearCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new EnemyAppearCommand());
    }
    m_troop_selection = command->enemy;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_troop_selection;

  bool m_confirmed{false};
  std::shared_ptr<EnemyAppearCommand> command;
  std::tuple<bool, bool> result;
};