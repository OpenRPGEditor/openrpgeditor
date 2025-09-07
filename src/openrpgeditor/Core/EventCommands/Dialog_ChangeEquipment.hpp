#pragma once
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Database/Actor.hpp"
#include "Database/Enemy.hpp"
#include "Database/EventCommands/ChangeEquipment.hpp"

struct Dialog_ChangeEquipment : IEventDialogController {
  Dialog_ChangeEquipment() = delete;
  explicit Dialog_ChangeEquipment(const std::string& name, const std::shared_ptr<ChangeEquipmentCommand>& cmd = nullptr)
  : IEventDialogController(name)
  , m_command(cmd) {
    if (cmd == nullptr) {
      m_command.reset(new ChangeEquipmentCommand());
    }
    m_actor = m_command->actorId;
    m_equipmentType = m_command->equipType;
    m_equipmentItem = m_command->equipment;
    makeEquipTypeList();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return m_command; };

private:
  void drawPickers();
  void makeEquipableItemList();
  void makeEquipTypeList();
  int m_equipmentType;
  int m_equipmentItem;
  int m_actor;

  bool m_confirmed{false};
  std::shared_ptr<ChangeEquipmentCommand> m_command;
  std::optional<ActorPicker> m_actorPicker;
  std::vector<int> m_equipableItems;
  std::vector<std::string> m_equipmentTypes;
};