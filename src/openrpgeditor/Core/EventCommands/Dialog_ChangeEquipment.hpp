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
    command.emplace();
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return std::make_shared<ChangeEquipmentCommand>(command.value()); };
  Project* m_project = nullptr;

private:
  int m_equipmentType = 1;
  int m_equipmentItem = 0;
  int m_actor = 1;

  bool m_confirmed{false};
  std::optional<ChangeEquipmentCommand> command;
  std::optional<ObjectPicker<Actor>> actor_picker;
  std::tuple<bool, bool> result;
};