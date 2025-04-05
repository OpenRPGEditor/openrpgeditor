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
  , command(cmd) {
    if (cmd == nullptr) {
      command.reset(new ChangeEquipmentCommand());
    }
    m_actor = command->actorId;
    m_equipmentType = command->equipType;
    m_equipmentItem = command->equipment;
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

private:
  int m_equipmentType;
  int m_equipmentItem;
  int m_actor;

  bool m_confirmed{false};
  std::shared_ptr<ChangeEquipmentCommand> command;
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::tuple<bool, bool> result;
};