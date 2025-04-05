#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeEnemyMP.hpp"
#include "Database/Troops.hpp"

struct Dialog_ChangeEnemyMP : IEventDialogController {
  Dialog_ChangeEnemyMP() = delete;
  explicit Dialog_ChangeEnemyMP(const std::string& name, const std::shared_ptr<ChangeEnemyMPCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeEnemyMPCommand());
    }

    m_enemy = static_cast<int>(command->enemy);
    m_enemyOp = static_cast<int>(command->enemyOp);
    m_quantitySource = static_cast<int>(command->quantitySource);

    if (command->quantitySource == QuantityChangeSource::Variable)
      m_quantity_var = command->quantity;
    else
      m_quantity = command->quantity;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_enemy;
  int m_enemyOp;
  int m_quantitySource;
  int m_quantity{1};
  int m_quantity_var{1};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Troop>> actor_picker;
  std::optional<VariableSwitchPicker> picker;
  std::shared_ptr<ChangeEnemyMPCommand> command;
  std::tuple<bool, bool> result;
};