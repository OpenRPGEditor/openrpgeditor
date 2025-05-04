#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/CommonUI/VariableSwitchPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/EventCommands/ChangeEnemyHP.hpp"
#include "Database/Troops.hpp"

struct Dialog_ChangeEnemyHP : IEventDialogController {
  Dialog_ChangeEnemyHP() = delete;
  explicit Dialog_ChangeEnemyHP(const std::string& name, const std::shared_ptr<ChangeEnemyHPCommand>& cmd = nullptr, const int troopId = 0)
  : IEventDialogController(name)
  , m_troopId(troopId)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeEnemyHPCommand());
    }

    m_troopMemberSelection = static_cast<int>(m_command->troopMember);
    m_troopMemberOp = static_cast<int>(m_command->troopMemberOp);
    m_quantitySource = static_cast<int>(m_command->quantitySource);
    m_allowKnockout = m_command->allowKnockOut;

    if (m_command->quantitySource == QuantityChangeSource::Variable)
      m_quantityVar = m_command->quantity;
    else
      m_quantity = m_command->quantity;
  }
  void drawPickers();
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  int m_troopId{0};
  int m_troopMemberSelection{0};
  int m_troopMemberOp{0};
  int m_quantitySource{0};
  int m_quantity{1};
  int m_quantityVar{1};
  bool m_allowKnockout{false};

  bool m_confirmed{false};
  std::optional<ObjectPicker<Troop>> m_actorPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeEnemyHPCommand> m_command;
};