#pragma once
#include "Core/EventCommands/IEventDialogController.hpp"
#include "Core/Project.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/EventCommands/CommonEvent.hpp"

using namespace std::string_view_literals;

struct Project;
struct Dialog_CommonEvent : IEventDialogController {
  Dialog_CommonEvent() = delete;
  explicit Dialog_CommonEvent(const std::string& name, Project* project) : IEventDialogController(name), m_project(project) {
    command.reset(new CommonEventCommand());
    eventId = command->event;
    m_picker = ObjectPicker("Common Events"sv, m_project->database().commonEvents.events(), 0);
  }
  std::tuple<bool, bool> draw() override;

  std::shared_ptr<IEventCommand> getCommand() override { return command; };

  Project* m_project = nullptr;

private:
  int eventId;
  bool m_confirmed{false};
  std::shared_ptr<CommonEventCommand> command;
  std::optional<ObjectPicker<std::optional<CommonEvent>>> m_picker;
  std::tuple<bool, bool> result;
};