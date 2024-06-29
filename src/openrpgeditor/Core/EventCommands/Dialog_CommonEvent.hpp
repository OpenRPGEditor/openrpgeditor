#pragma once
#include "IDialogController.hpp"
#include "Core/Project.hpp"
#include "Core/CommonUI/ObjectPicker.hpp"
#include "Database/CommonEvents.hpp"

using namespace std::string_view_literals;

struct Project;
struct Dialog_CommonEvent : IDialogController {
  Dialog_CommonEvent() = delete;
  explicit Dialog_CommonEvent(const std::string& name, Project* project) : IDialogController(name), m_project(project) {
      ce_picker = ObjectPicker<std::optional<CommonEvent>>("Common Events"sv, m_project->database().commonEvents.events(), 0);
  }
  std::tuple<bool, bool>  draw() override;

  std::optional<CommonEvent> getCommandData() { return command; }
  Project* m_project = nullptr;
private:
  int d_common_id = 1;
  std::optional<CommonEvent> command;
  std::optional<ObjectPicker<std::optional<CommonEvent>>> ce_picker;
  std::tuple<bool, bool> result;
};