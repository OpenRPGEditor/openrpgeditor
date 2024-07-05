#pragma once
#include "IDialogController.hpp"
#include "Core/Project.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/CommonEvents.hpp"
#include "Database/EventCommands/CommonEvent.hpp"

using namespace std::string_view_literals;

struct Project;
struct Dialog_CommonEvent : IDialogController {
  Dialog_CommonEvent() = delete;
  explicit Dialog_CommonEvent(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
command = new CommonEventCommand();
    eventId = command->event;
      ce_picker = ObjectPicker("Common Events"sv, m_project->database().commonEvents.events(), 0);
  }
  std::tuple<bool, bool>  draw() override;

  IEventCommand* getCommand() override {
    return command;
  };

  Project* m_project = nullptr;
private:
  int eventId;
  bool m_confirmed{false};
CommonEventCommand* command;
  std::optional<ObjectPicker<std::optional<CommonEvent>>> ce_picker;
  std::tuple<bool, bool> result;
};