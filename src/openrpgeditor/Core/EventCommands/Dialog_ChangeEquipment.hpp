#pragma once
#include "IDialogController.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/Actors.hpp"
#include "Database/Enemies.hpp"
#include "Database/EventCommands/ChangeEquipment.hpp"

struct Project;
struct Dialog_ChangeEquipment : IDialogController {
  Dialog_ChangeEquipment() = delete;
  explicit Dialog_ChangeEquipment(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new ChangeEquipmentCommand();
    m_actor = command->actorId;
    m_equipmentType = command->equipType;
    m_equipmentItem = command->equipment;
  }
  std::tuple<bool, bool> draw() override;

  IEventCommand* getCommand() override { return command; };
  Project* m_project = nullptr;

private:
  int m_equipmentType;
  int m_equipmentItem;
  int m_actor;

  bool m_confirmed{false};
ChangeEquipmentCommand* command;
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::tuple<bool, bool> result;
};