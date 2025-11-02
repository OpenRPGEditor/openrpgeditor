#pragma once
#include "Database/EventCommands/ChangeArmors.hpp"
#include "Database/EventCommands/ChangeEnemyHP.hpp"
#include "Database/Troops.hpp"
#include "Editor/CommonUI/ObjectPicker.hpp"
#include "Editor/CommonUI/VariableSwitchPicker.hpp"
#include "Editor/EventCommands/IEventDialogController.hpp"

struct Armor;
struct Dialog_ChangeArmors : IEventDialogController {
  Dialog_ChangeArmors() = delete;
  explicit Dialog_ChangeArmors(const std::string& name, const std::shared_ptr<ChangeArmorsCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeArmorsCommand());
    }

    m_item = m_command->item;
    m_operation = static_cast<int>(m_command->operation);
    m_operandSource = static_cast<int>(m_command->operandSource);
    m_includeEquipment = m_command->includeEquipment;

    if (m_command->operandSource == QuantityChangeSource::Variable)
      m_quantityVar = m_command->operand;
    else
      m_quantity = m_command->operand;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  int m_item;
  int m_operation;
  int m_operandSource;
  int m_quantity{1};
  int m_quantityVar{1};
  bool m_includeEquipment{false};

  bool m_confirmed{false};
  std::optional<ArmorPicker> m_armorPicker;
  std::optional<VariableSwitchPicker> m_variablePicker;
  std::shared_ptr<ChangeArmorsCommand> m_command;
};