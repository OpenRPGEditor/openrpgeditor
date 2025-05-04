#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeEnemyTP.hpp"
#include "Database/Troops.hpp"

struct Dialog_ChangeEnemyTP : IEventDialogController {
  Dialog_ChangeEnemyTP() = delete;
  explicit Dialog_ChangeEnemyTP(const std::string& name, const std::shared_ptr<ChangeEnemyTPCommand>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeEnemyTPCommand());
    }

    m_troopMemberSelection = m_command->troopMember;
    m_troopMemberOp = static_cast<int>(m_command->troopMemberOp);
    m_quantitySource = static_cast<int>(m_command->quantitySource);

    if (m_command->quantitySource == QuantityChangeSource::Variable)
      m_quantityVar = m_command->quantity;
    else
      m_quantity = m_command->quantity;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_troopId;
  int m_troopMemberSelection;
  int m_troopMemberOp;
  int m_quantitySource;
  int m_quantity{1};
  int m_quantityVar{1};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Troop>> m_actorPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeEnemyTPCommand> m_command;
};